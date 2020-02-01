








#ifndef _FMOD_OUTPUT_H
#define _FMOD_OUTPUT_H

#define FMOD_OUTPUT_PLUGIN_VERSION 3

typedef struct FMOD_OUTPUT_STATE        FMOD_OUTPUT_STATE;
typedef struct FMOD_OUTPUT_OBJECT3DINFO FMOD_OUTPUT_OBJECT3DINFO;

 
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETNUMDRIVERS_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, int *numdrivers);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETDRIVERINFO_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_INIT_CALLBACK)             (FMOD_OUTPUT_STATE *output_state, int selecteddriver, FMOD_INITFLAGS flags, int *outputrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels, FMOD_SOUND_FORMAT *outputformat, int dspbufferlength, int dspnumbuffers, void *extradriverdata);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_START_CALLBACK)            (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_STOP_CALLBACK)             (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_CLOSE_CALLBACK)            (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_UPDATE_CALLBACK)           (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETHANDLE_CALLBACK)        (FMOD_OUTPUT_STATE *output_state, void **handle);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETPOSITION_CALLBACK)      (FMOD_OUTPUT_STATE *output_state, unsigned int *pcm);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_LOCK_CALLBACK)             (FMOD_OUTPUT_STATE *output_state, unsigned int offset, unsigned int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_UNLOCK_CALLBACK)           (FMOD_OUTPUT_STATE *output_state, void *ptr1, void *ptr2, unsigned int len1, unsigned int len2);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_MIXER_CALLBACK)            (FMOD_OUTPUT_STATE *output_state);

typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DGETINFO_CALLBACK)  (FMOD_OUTPUT_STATE *output_state, int *maxhardwareobjects);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DALLOC_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, void **object3d);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DFREE_CALLBACK)     (FMOD_OUTPUT_STATE *output_state, void *object3d);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DUPDATE_CALLBACK)   (FMOD_OUTPUT_STATE *output_state, void *object3d, const FMOD_OUTPUT_OBJECT3DINFO *info);

typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OPENPORT_CALLBACK)         (FMOD_OUTPUT_STATE *output_state, FMOD_PORT_TYPE portType, FMOD_PORT_INDEX portIndex, int *portId, int *portRate, int *portChannels, FMOD_SOUND_FORMAT *portFormat);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_CLOSEPORT_CALLBACK)        (FMOD_OUTPUT_STATE *output_state, int portId);



typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_READFROMMIXER_FUNC)        (FMOD_OUTPUT_STATE *output_state, void *buffer, unsigned int length);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_COPYPORT_FUNC)             (FMOD_OUTPUT_STATE *output_state, int portId, void *buffer, unsigned int length);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_REQUESTRESET_FUNC)         (FMOD_OUTPUT_STATE *output_state);
typedef void *      (F_CALL *FMOD_OUTPUT_ALLOC_FUNC)                (unsigned int size, unsigned int align, const char *file, int line);
typedef void        (F_CALL *FMOD_OUTPUT_FREE_FUNC)                 (void *ptr, const char *file, int line);
typedef void        (F_CALL *FMOD_OUTPUT_LOG_FUNC)                  (FMOD_DEBUG_FLAGS level, const char *file, int line, const char *function, const char *string, ...);



typedef struct FMOD_OUTPUT_DESCRIPTION
{
    unsigned int                            apiversion;         
    const char                             *name;               
    unsigned int                            version;            
    int                                     polling;            
    FMOD_OUTPUT_GETNUMDRIVERS_CALLBACK      getnumdrivers;      
    FMOD_OUTPUT_GETDRIVERINFO_CALLBACK      getdriverinfo;      
    FMOD_OUTPUT_INIT_CALLBACK               init;               
    FMOD_OUTPUT_START_CALLBACK              start;              
    FMOD_OUTPUT_STOP_CALLBACK               stop;               
    FMOD_OUTPUT_CLOSE_CALLBACK              close;              
    FMOD_OUTPUT_UPDATE_CALLBACK             update;             
    FMOD_OUTPUT_GETHANDLE_CALLBACK          gethandle;          
    FMOD_OUTPUT_GETPOSITION_CALLBACK        getposition;        
    FMOD_OUTPUT_LOCK_CALLBACK               lock;               
    FMOD_OUTPUT_UNLOCK_CALLBACK             unlock;             
    FMOD_OUTPUT_MIXER_CALLBACK              mixer;              
    FMOD_OUTPUT_OBJECT3DGETINFO_CALLBACK    object3dgetinfo;    
    FMOD_OUTPUT_OBJECT3DALLOC_CALLBACK      object3dalloc;      
    FMOD_OUTPUT_OBJECT3DFREE_CALLBACK       object3dfree;       
    FMOD_OUTPUT_OBJECT3DUPDATE_CALLBACK     object3dupdate;     
    FMOD_OUTPUT_OPENPORT_CALLBACK           openport;           
    FMOD_OUTPUT_CLOSEPORT_CALLBACK          closeport;          
} FMOD_OUTPUT_DESCRIPTION;



struct FMOD_OUTPUT_STATE
{
    void                            *plugindata;     
    FMOD_OUTPUT_READFROMMIXER_FUNC   readfrommixer;  
    FMOD_OUTPUT_ALLOC_FUNC           alloc;          
    FMOD_OUTPUT_FREE_FUNC            free;           
    FMOD_OUTPUT_LOG_FUNC             log;            
    FMOD_OUTPUT_COPYPORT_FUNC        copyport;       
    FMOD_OUTPUT_REQUESTRESET_FUNC    requestreset;   
};



struct FMOD_OUTPUT_OBJECT3DINFO
{
    float          *buffer;         
    unsigned int    bufferlength;   
    FMOD_VECTOR     position;       
    float           gain;           
    float           spread;         
    float           priority;       
};



#define FMOD_OUTPUT_READFROMMIXER(_state, _buffer, _length) \
    (_state)->readfrommixer(_state, _buffer, _length)
#define FMOD_OUTPUT_ALLOC(_state, _size, _align) \
    (_state)->alloc(_size, _align, __FILE__, __LINE__)
#define FMOD_OUTPUT_FREE(_state, _ptr) \
    (_state)->free(_ptr, __FILE__, __LINE__)
#define FMOD_OUTPUT_LOG(_state, _level, _location, _format, ...) \
    (_state)->log(_level, __FILE__, __LINE__, _location, _format, __VA_ARGS__)
#define FMOD_OUTPUT_COPYPORT(_state, _id, _buffer, _length) \
    (_state)->copyport(_state, _id, _buffer, _length)
#define FMOD_OUTPUT_REQUESTRESET(_state) \
    (_state)->requestreset(_state)

#endif 