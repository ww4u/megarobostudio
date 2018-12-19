#include "encoderalarmpage.h"
#include "ui_encoderalarmpage.h"

#include "../../device/mrq/deviceMRQ.h"
#include "../../com/comassist.h"
EncoderAlarmPage::EncoderAlarmPage(QWidget *parent) :
    modelView(parent),
    ui(new Ui::EncoderAlarmPage)
{
    ui->setupUi(this);

    //! set name
    ui->label_2->setText( tr("Upper") + "(" + char_deg + ")" );
    ui->label_3->setText( tr("Lower") + "(" + char_deg + ")" );
    ui->label_4->setText( tr("Zero") + "(" + char_deg + ")" );

    spyEdited();
}

EncoderAlarmPage::~EncoderAlarmPage()
{
    delete ui;
}

void EncoderAlarmPage::changeEvent( QEvent *event )
{
    modelView::changeEvent( event );

    if ( event->type() == QEvent::LanguageChange )
    { ui->retranslateUi( this ); }
}

#define angle_error     (1e-6)

#define align_angle_value( angle )    \
                                        ( ( \
                                         MegaDevice::deviceMRQ::s_absAngleToValue( angle)\
                                         ) )

#define align_value_angle( value )     ( ( \
                                        MegaDevice::deviceMRQ::s_valueToAbsAngle( value ) \
                                        ) )

void EncoderAlarmPage::setData( EncoderAlarmConfig &config )
{
    float valDown, valUp;

    ui->checkBox->setChecked( config.mbEn );

    //! vals from reg
    valDown = align_angle_value( config.mDownLimit);
    valUp = align_angle_value(config.mUpLimit);

    ui->doubleSpinBox->setValue(  valUp );
    ui->doubleSpinBox_2->setValue(  valDown );

    ui->doubleSpinBox_3->setValue( align_angle_value(config.mZero) );
}
void EncoderAlarmPage::data( EncoderAlarmConfig &config )
{
    float limitL, limitU;

    limitU = ui->doubleSpinBox->value();
    limitL = ui->doubleSpinBox_2->value();

    config.mbEn = ui->checkBox->isChecked();

    config.mUpLimit =  align_value_angle( limitU );
    config.mDownLimit =  align_value_angle( limitL );

    config.mZero =  align_value_angle( ui->doubleSpinBox_3->value() );
}

void EncoderAlarmPage::setZeroEnable( bool b )
{ ui->doubleSpinBox_3->setEnabled( b ); }
bool EncoderAlarmPage::isZeroEnable()
{ return ui->doubleSpinBox_3->isEnabled(); }

void EncoderAlarmPage::spyEdited()
{
    QCheckBox *checkBoxes[]=
    {
        ui->checkBox,
    };
    QRadioButton *radBoxes[] = {
    };

    QLineEdit *edits[]={

    };

    QSpinBox *spinBoxes[]={

    };
    QDoubleSpinBox *doubleSpinBoxes[]={
        ui->doubleSpinBox,
        ui->doubleSpinBox_2,
        ui->doubleSpinBox_3,
    };

    QComboBox *comboxes[]={
//        ui->comboBox,
    };

    install_spy();
}

void EncoderAlarmPage::limitToRange( float valDown, float valUp,
                   float zero,
                   bool bZeroRoll,
                   float &rangeL, float &rangeU
                   )
{

    if ( bZeroRoll )
    {
        //!  -------  down --- up  --- zero
        if ( zero >= valUp )
        {
            rangeL = valUp - zero;
            rangeU = ( valDown + 360 ) - zero;
        }
        //!  zero  down ---- up ---
        else
        {
            rangeU = valDown - zero;
            rangeL = valUp - ( zero + 360 );
        }
    }
    //! down   --- zero --- up
    else
    {
        rangeL = valDown - zero;
        rangeU = valUp - zero;
    }
}

void EncoderAlarmPage::rangeToLimit( float rangeL, float rangeU,
                   float zero,
                   bool & bZeroRoll,
                   float &valL, float &valU )
{
    valL = zero + rangeL;
    valU = zero + rangeU;

    valL = comAssist::normalizeDegree360( valL );
    valU = comAssist::normalizeDegree360( valU );

    //! sort the l,u
    if ( valL  <= valU )
    {
    }
    else
    {
        qSwap( valL, valU );
    }

    //! find the range
    if ( valL <= zero && zero <= valU )
    { bZeroRoll = false; }
    else
    { bZeroRoll = true; }
}
