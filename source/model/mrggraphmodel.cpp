#include "mrggraphmodel.h"

MrgGraphModel::MrgGraphModel()
{

}

int MrgGraphModel::load( const QString &name )
{
    //! \todo clean the previous?

    return mModel.load( name );
}
int MrgGraphModel::save( const QString &name )
{
    return mModel.save( name );
}
