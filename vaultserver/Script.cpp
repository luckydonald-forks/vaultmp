#include "Script.h"

/* AMX extension modules */
#define FLOATPOINT // floating point for console
#define AMXCONSOLE_NOIDLE // no idle state
#define AMXTIME_NOIDLE // no idle state
#define NDEBUG // no debug
#include "amx/amxcore.c"
#undef __T
#include "amx/amxfile.c"
#undef __T
#include "amx/amxcons.c"
#undef __T
#include "amx/amxstring.c"
#include "amx/amxtime.c"
#include "amx/float.c"

void Script::ErrorExit(AMX* amx, int errorcode)
{
    Utils::timestamp();
    printf("Run time error %d: \"%s\"\n", errorcode, aux_StrError(errorcode));
    exit(1);
}

int Script::LoadProgram(AMX* amx, char* filename, void* memblock)
{
    return aux_LoadProgram(amx, filename, memblock);
}

int Script::Register(AMX* amx, const AMX_NATIVE_INFO* list, int number)
{
    return amx_Register(amx, list, number);
}

int Script::Exec(AMX* amx, cell* retval, int index)
{
    return amx_Exec(amx, retval, index);
}

int Script::Call(AMX* amx, char name[], char argl[], void* args[])
{
    cell ret = 0;
    int idx = 0;
    int err = 0;

    err = amx_FindPublic(amx, name, &idx);
    if (err != AMX_ERR_NONE)
        ErrorExit(amx, err);

    cell stackpointer = 0;

    for (int i = 0; i < sizeof(argl); i++)
    {
        switch (argl[i])
        {
            case 'i':
            {
                break;
            }
            case 'c':
            {
                break;
            }
            case 'f':
            {
                break;
            }
            case 's':
            {
                char* string = reinterpret_cast<char*>(args[i]);

                if (stackpointer != 0) amx_PushString(amx, NULL, NULL, string, 0, 0);
                else amx_PushString(amx, &stackpointer, NULL, string, 0, 0);
                break;
            }
        }
    }

    err = amx_Exec(amx, &ret, idx);
    if (err != AMX_ERR_NONE)
        ErrorExit(amx, err);

    amx_Release(amx, stackpointer);

    return ret;
}

int Script::FreeProgram(AMX* amx)
{
    return aux_FreeProgram(amx);
}

int Script::CoreInit(AMX* amx)
{
    return amx_CoreInit(amx);
}

int Script::ConsoleInit(AMX* amx)
{
    return amx_ConsoleInit(amx);
}

int Script::FloatInit(AMX* amx)
{
    return amx_FloatInit(amx);
}

int Script::TimeInit(AMX* amx)
{
    return amx_TimeInit(amx);
}

int Script::StringInit(AMX* amx)
{
    return amx_StringInit(amx);
}

int Script::FileInit(AMX* amx)
{
    return amx_FileInit(amx);
}
