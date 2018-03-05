
#ifndef _MC_ERROR_H_
#define _MC_ERROR_H_

enum mcError {
	ERR_NONE = 0,
	
	ERR_NULL_POINTER,
        ERR_ALLOC_FAIL,
	ERR_INVALID_INPUT,
	ERR_OVER_RAGNE,
	ERR_OVER_UPPER,
	ERR_OVER_LOWER,

        ERR_FILE_OPEN_FAIL,
        ERR_FILE_WRITE_FAIL,
        ERR_FILE_READ_FAIL,
        ERR_FILE_DO_NOT_SUPPORT,
        ERR_FILE_NAME,

        ERR_INVALID_SCRIPT_FILE_NODE,

        ERR_INVALID_DEVICE_NAME,
        ERR_INVALID_ROBOT_NAME,

        ERR_DEVICE_WRITE_FAIL,

        ERR_NO_TPV_DATA,

        ERR_INVALID_TPV_T,
        ERR_INVALID_TPV_COUNT,
        ERR_INVALID_TPV_CMD,

        ERR_FETCH_FAIL,
        ERR_PLAN_FAIL,
        ERR_PLAN_SPLIT_FAIL,
};

#endif
