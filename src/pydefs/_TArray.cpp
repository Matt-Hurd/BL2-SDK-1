#include "stdafx.h"

#include "gamedefines.h"
// Using =======================================================================
namespace py = pybind11;

// Module ======================================================================
void Export_pystes_TArray(py::module_ &m)
{
	py::class_< PyTArray>(m, "PyTArray", py::dynamic_attr());
}
