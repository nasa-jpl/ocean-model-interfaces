#ifndef MODEL_DATA_PY_INTERFACE
#define MODEL_DATA_PY_INTERFACE

extern "C"
{
    struct ModelDataPy {
        double u;
        double v;
        double w;
        double temp;
        double salt;
        double dye;
        double depth;
    };
}

#endif