#ifndef _MC_CFG_H_
#define _MC_CFG_H_

//! 设备过程中发出线程信号
#define DEVICE_EMIT_SIGNAL		1

//! ref to applicationDirPath()
#define DEVICE_TALBE_NAME               QCoreApplication::applicationDirPath() + "/data/drivercontroller.xml"

//! config
#define pref_file_name                  QCoreApplication::applicationDirPath() + "/data/pref.xml"
#define robo_mgr_name                   QCoreApplication::applicationDirPath() + "/data/robot.xml"

//! loops
#define min_loops                       1
#define max_loops                       1000000000

//! id range
#define receive_id_from                 0x180
#define receive_id_to                   0x1ff

#define send_id_from                    0x200
#define send_id_to                      0x27f

#define group_id_from                   0x80
#define group_id_to                     0xff
#define group_segment                   0x4

#define sub_group_id_from               130
#define sub_group_id_to                 255

#define x_channel                       128
#define x_pages                         10
//#define robo_channels(subGpIndex)            ( subGpIndex + sub_group_id_from + 10 )

#define CHAR_DEGREE                        QChar(0x00B0)

//! file ext
#define setup_ext                       "stp"
#define motion_ext                      "mc"
#define pvt_ext                         "pvt"
#define pt_ext                          "pt"
#define scene_ext                       "sce"

#define script_ext                      "py"

//! file dext
#define setup_d_ext                       ".stp"
#define motion_d_ext                      ".mc"
#define pvt_d_ext                         ".pvt"
#define scene_d_ext                       ".sce"

#define script_d_ext                      ".py"

//! file description
#define setup_desc                       "Setup file"
#define motion_desc                      "Motion filc"
#define pvt_desc                         "PVT file"
#define pt_desc                          "PT file"
#define scene_desc                       "Scene file"

#define script_desc                      "Python file"

#define all_desc                         "All file"

#define CSV_LINE_SEP                    "\n"
#define CSV_COL_SEP                     ","


//! robo name
#define robo_t4     "MRX-T4"
#define robo_dt     "MRX-DT"
#define robo_as     "MRX-AS"
#define robo_h2     "MRX-H2"

#define robo_h2m    "MRX-H2M"
#define robo_h2z    "MRX-H2Z"
#define robo_ip     "MRX-INJ2"

#endif
