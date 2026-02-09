#include "Module.h"
#include "Basic.h"

PYBIND11_MODULE(PyEngine, m, "MyEngine for Python (Used Pybind11)") {
    m.attr("__PyEngine_version__") = MYENGINE_FULL_VERSION;
    m.attr("__PyEngine_major_version__") = MYENGINE_MAJOR_VERSION;
    m.attr("__PyEngine_minor_version__") = MYENGINE_MINOR_VERSION;
    m.attr("__PyEngine_patch_version__") = MYENGINE_PATCH_VERSION;
    m.def("version", []{ return MYENGINE_FULL_VERSION; });

    initBasic(m);
}
