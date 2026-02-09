
#include "Basic.h"

void initBasic(pybind11::module_ &m) {
    m.def("delay", &delay);
    m.def("delayMS", &delayMS);
    m.def("delayNS", &delayNS);
    m.def("delayUS", &delayUS);

    py::module_ id_generator = m.def_submodule("IDGenerator", "Generate unique ID. "
                    "Multiple ID generators can be created and generate unique ID values.");
    id_generator.def("newIDGenerator", &IDGenerator::newIDGenerator);
    id_generator.def("getNewEventID", &IDGenerator::getNewEventID);
    id_generator.def("getNewGlobalEventID", &IDGenerator::getNewGlobalEventID);
    id_generator.def("getNewTextID", &IDGenerator::getNewTextID);
    id_generator.def("getID", &IDGenerator::getID, "Use the specified ID Generator to get the new ID.",
        py::arg("index") = 0);

    py::class_<Geometry>(m, "Geometry")
        .def(py::init<>())
        .def(py::init<int, int, int, int>(), py::arg("x"), py::arg("y"),
                                                 py::arg("width"), py::arg("height"))
        .def_readwrite("x", &Geometry::x)
        .def_readwrite("y", &Geometry::y)
        .def_readwrite("width", &Geometry::width)
        .def_readwrite("height", &Geometry::height)
        .def("__repr__", [](const Geometry& g) {
            return py::str("Geometry({}, {}, {}, {})").format(g.x, g.y, g.width, g.height);
        })
        .def("setGeometry", static_cast<void (Geometry::*)(int, int, int, int)>(&Geometry::setGeometry))
        .def("setGeometry", static_cast<void (Geometry::*)(const Geometry&)>(&Geometry::setGeometry))
        .def("move", &Geometry::move)
        .def("resize", &Geometry::resize);

    py::class_<Vector2>(m, "Vector2")
        .def(py::init<>())
        .def(py::init<float, float>(), py::arg("x"), py::arg("y"))
        .def_readwrite("x", &Vector2::x)
        .def_readwrite("y", &Vector2::y)
        .def("__repr__", [](const Vector2& v) { return py::str("Vector2({:.2f}, {:.2f})").format(v.x, v.y); })
        .def("reset", static_cast<void (Vector2::*)(float, float)>(&Vector2::reset))
        .def("reset", static_cast<void (Vector2::*)(const Vector2&)>(&Vector2::reset))
        .def("isEqual", &Vector2::isEqual, py::arg("other"), py::arg("EPISON") = 1e-6f);

    py::class_<Size>(m, "Size")
        .def(py::init<>())
        .def(py::init<float, float>(), py::arg("width"), py::arg("height"))
        .def_readwrite("width", &Size::width)
        .def_readwrite("height", &Size::height)
        .def("__repr__", [](const Size& size) { return py::str("Size({:.2f} x {:.2f})")
                                                                       .format(size.width, size.height); })
        .def("reset", static_cast<void (Size::*)(float, float)>(&Size::reset))
        .def("reset", static_cast<void (Size::*)(const Size&)>(&Size::reset));

    py::class_<GeometryF>(m, "GeometryF")
        .def(py::init<>())
        .def(py::init<float, float, float, float>(), py::arg("x"), py::arg("y"),
                                                 py::arg("width"), py::arg("height"))
        .def(py::init<Vector2, Size>(), py::arg("position"), py::arg("size"))
        .def_readwrite("pos", &GeometryF::pos)
        .def_readwrite("size", &GeometryF::size)
        .def("__repr__", [](const GeometryF& g) {
            return py::str("GeometryF({:.2f}, {:.2f}, {:.2f}, {:.2f})").format(g.pos.x, g.pos.y,
                                                                                 g.size.width, g.size.height);
        })
        .def("setGeometry", static_cast<void (GeometryF::*)(float, float, float, float)>(&GeometryF::setGeometry),
            py::arg("x"), py::arg("y"), py::arg("width"), py::arg("height"))
        .def("setGeometry", static_cast<void (GeometryF::*)(const Vector2&, const Size&)>(&GeometryF::setGeometry),
            py::arg("position"), py::arg("size"))
        .def("setGeometry", static_cast<void (GeometryF::*)(const GeometryF&)>(&GeometryF::setGeometry),
            py::arg("geometry"))
        .def("move", static_cast<void (GeometryF::*)(float, float)>(&GeometryF::move), py::arg("x"), py::arg("y"))
        .def("move", static_cast<void (GeometryF::*)(const Vector2&)>(&GeometryF::move), py::arg("position"))
        .def("resize", static_cast<void (GeometryF::*)(float, float)>(&GeometryF::resize), py::arg("width"), py::arg("height"))
        .def("resize", static_cast<void (GeometryF::*)(const Size&)>(&GeometryF::resize), py::arg("size"));

}
