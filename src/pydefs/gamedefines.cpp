#include "stdafx.h"

#include "UnrealEngine/Core/UE3/Core_classes.h"

// Using =======================================================================
namespace py = pybind11;

// Module ======================================================================
void Export_pystes_gamedefines(py::module_ &m)
{
	py::class_< FName >(m, "FName")
		.def(py::init<>())
		.def(py::init<const std::string&>())
		.def(py::init<const std::string&, int>())
		.def_static("Names", &FName::Names, py::return_value_policy::reference)
		.def_readwrite("Index", &FName::Index)
		.def_readwrite("Number", &FName::Number)
		.def("GetName", &FName::GetName, py::return_value_policy::reference)
		;
	py::class_< FNameEntry >(m, "FNameEntry")
		.def(py::init<>())
		.def_readonly("Name", &FNameEntry::AnsiName)
		;
	py::class_< FOutputDevice >(m, "FOutputDevice")
		.def(py::init<>())
		.def_readwrite("VfTable", &FOutputDevice::VfTable, py::return_value_policy::reference)
		#ifndef UE4
		.def_readwrite("bAllowSuppression", &FOutputDevice::bAllowSuppression, py::return_value_policy::reference)
		#endif
		.def_readwrite("bSuppressEventTag", &FOutputDevice::bSuppressEventTag, py::return_value_policy::reference)
		.def_readwrite("bAutoEmitLineTerminator", &FOutputDevice::bAutoEmitLineTerminator, py::return_value_policy::reference)
		;
	py::class_< FFrame, FOutputDevice >(m, "FFrame")
		.def_readwrite("Node", &FFrame::Node, py::return_value_policy::reference)
		.def_readwrite("Object", &FFrame::Object, py::return_value_policy::reference)
		.def_readwrite("PreviousFrame", &FFrame::PreviousFrame, py::return_value_policy::reference)
		.def_property("Code", [](FFrame &self) { return (int)self.Code; }, [](FFrame &self, int value) {self.Code = (unsigned char *)value; })
		.def_property("Locals", [](FFrame &self) { return (int)self.Locals; }, [](FFrame &self, int value) {self.Locals = (unsigned char *)value; })
		.def("SkipFunction", &FFrame::SkipFunction, py::return_value_policy::reference)
		;

	py::class_< FString >(m, "FString")
		.def(py::init<wchar_t*>())
		.def_readwrite("Count", &FString::Count)
		.def_readwrite("Max", &FString::Max)
		.def_readwrite("Data", &FString::Data)
		.def("AsString", &FString::AsString)
		;

	py::class_< FScriptDelegate >(m, "FScriptDelegate")
#ifdef UE3
		.def(py::init<>())
#else
		.def(py::init<UObject*, FName>())
		.def("IsBound", &FScriptDelegate::IsBound)
		.def("ToString", &FScriptDelegate::ToString)
		.def("__repr__", &FScriptDelegate::ToString)
#endif
		;

#ifdef UE4
	py::class_< FSoftObject<UObject> >(m, "FSoftObject")
		.def_readwrite("object", &FSoftObject<UObject>::object)
		.def_readwrite("asset_path", &FSoftObject<UObject>::asset_path);

	py::class_< FSoftObject<UClass> >(m, "FSoftClass")
		.def_readwrite("object", &FSoftObject<UClass>::object)
		.def_readwrite("asset_path", &FSoftObject<UClass>::asset_path);
#endif
}
