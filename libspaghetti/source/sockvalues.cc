#include <spaghetti/sockvalues.h>
#include <spaghetti/utils.h>

namespace spaghetti {

std::vector<ValueTypeDescription> const g_valuesDescriptions{
  // clang-format off
ValueTypeDescription{
    ValueType::eBool,
    IOSocketFlags::eCanHoldBool,
    "bool",
    "Bool",
    { { 244, 53, 64, 255 }, { 75, 173, 88, 255 } },
    false
},
ValueTypeDescription{
    ValueType::eInt,
    IOSocketFlags::eCanHoldInt,
    "int",
    "Int",
    { { 0, 170, 238, 255 }, { 0, 170, 238, 255 } },
    0
},
ValueTypeDescription{
    ValueType::eIntVector,
    IOSocketFlags::eCanHoldIntVector,
    "int_vector",
    "IntVector",
    { { 0, 120, 208, 255 }, { 0, 120, 208, 255 } },
    IntVector()
},
ValueTypeDescription{
    ValueType::eFloat,
    IOSocketFlags::eCanHoldFloat,
    "float",
    "Float",
    { { 254, 144, 50, 255 }, { 111, 80, 96, 255 } },
    0.0f
},
ValueTypeDescription{
    ValueType::eFloatVector,
    IOSocketFlags::eCanHoldFloatVector,
    "float_vector",
    "FloatVector",
    { { 168, 117, 50, 255 }, { 168, 117, 50, 255 } },
    FloatVector()
},
ValueTypeDescription{
    ValueType::eString,
    IOSocketFlags::eCanHoldString,
    "string",
    "String",
    { { 95, 124, 136, 255 }, { 95, 124, 136, 255 } },
    std::string{}
},
ValueTypeDescription{
    ValueType::eStringVector,
    IOSocketFlags::eCanHoldStringVector,
    "string_vector",
    "StringVector",
    { { 162, 189, 245, 255 }, { 162, 189, 245, 255 } },
    StringVector()
},
ValueTypeDescription{
    ValueType::eMatrix,
    IOSocketFlags::eCanHoldMatrix,
    "matrix",
    "Matrix",
    { { 169, 169, 169, 255 }, { 169, 169, 169, 255 } },
    cv::Mat()
},
ValueTypeDescription{
    ValueType::eMatrixVector,
    IOSocketFlags::eCanHoldMatrixVector,
    "matrix_vector",
    "MatrixVector",
    { { 82, 56, 54, 255 }, { 82, 56, 54, 255 } },
    MatrixVector()
},
ValueTypeDescription{
    ValueType::ePoint,
    IOSocketFlags::eCanHoldPoint,
    "point",
    "Point",
    { { 201, 57, 237, 255 }, { 201, 57, 237, 255 } },
    cv::Point()
},
ValueTypeDescription{
    ValueType::eShape,
    IOSocketFlags::eCanHoldShape,
    "shape",
    "Shape",
    { { 57, 237, 222, 255 }, { 57, 237, 222, 255 } },
    Shape()
},
ValueTypeDescription{
    ValueType::eShapeVector,
    IOSocketFlags::eCanHoldShapeVector,
    "shape_vector",
    "ShapeVector",
    { { 207, 237, 57, 255 }, { 207, 237, 57, 255 } },
    ShapeVector()
},
ValueTypeDescription{
    ValueType::eDNNData,
    IOSocketFlags::eCanHoldDNNData,
    "dnn_data",
    "DNNData",
    { { 14, 204, 160, 255 }, { 14, 204, 160, 255 } },
    DNNData()
},
ValueTypeDescription{
    ValueType::eDNNRect,
    IOSocketFlags::eCanHoldDNNRect,
    "dnn_rect",
    "DNNRect",
    { { 179, 135, 230, 255 }, { 179, 135, 230, 255 } },
    DNNRect()
},
ValueTypeDescription{
    ValueType::eDNNRectVector,
    IOSocketFlags::eCanHoldDNNRectVector,
    "dnn_rect_vector",
    "DNNRectVector",
    { { 119, 75, 170, 255 }, { 179, 135, 230, 255 } },
    DNNRectVector()
},
ValueTypeDescription{
    ValueType::eDataArray,
    IOSocketFlags::eCanHoldDataArray,
    "data_array",
    "DataArray",
    { { 64, 99, 73, 255 }, { 22, 59, 32, 255 } },
    DataArray()
},
ValueTypeDescription{
    ValueType::eDNNPath,
    IOSocketFlags::eCanHoldDNNPath,
    "dnn_path",
    "DNNPath",
    { { 202, 250, 228, 255 }, { 202, 250, 228, 255 } },
    DNNPath()
},
ValueTypeDescription{
    ValueType::eDNNPathVector,
    IOSocketFlags::eCanHoldDNNPathVector,
    "dnn_path_vector",
    "DNNPathVector",
    { { 113, 145, 130, 255 }, { 113, 145, 130, 255 } },
    DNNPathVector()
},
ValueTypeDescription{
    ValueType::eDNNPathInfo,
    IOSocketFlags::eCanHoldDNNPathInfo,
    "dnn_path_info",
    "DNNPathInfo",
    { { 199, 142, 28, 255 }, { 199, 142, 28, 255 } },
    DNNPathInfo()
},
ValueTypeDescription{
    ValueType::eDNNPathInfoVector,
    IOSocketFlags::eCanHoldDNNPathInfoVector,
    "dnn_path_info_vector",
    "DNNPathInfoVector",
    { { 128, 91, 18, 255 }, { 128, 91, 18, 255 } },
    DNNPathInfoVector()
},
ValueTypeDescription{
    ValueType::eCable,
    IOSocketFlags::eCanHoldCable,
    "cable",
    "Cable",
    { { 0, 0, 0, 255 }, { 255, 146, 3, 255 } },
    Cable()
}
  // clang-format on
};

ValueTypeDescription::ValueTypeDescription(ValueType const a_type, IOSocketFlags::Flags const a_holdType,
                                           std::string const a_typeString, std::string const a_typeQstring,
                                           std::pair<ElementColor, ElementColor> const a_colors, Value const a_value)
  : type{ a_type }
  , holdType{ a_holdType }
  , typeString{ a_typeString }
  , typeQstring{ a_typeQstring }
  , colors{ a_colors }
  , value{ a_value }
{
}

std::string ValueDescription::typeStdString(ValueType const a_type)
{
  auto typeID = static_cast<size_t>(a_type);
  assert(typeID < g_valuesDescriptions.size());

  return g_valuesDescriptions[typeID].typeQstring;
}

const char *ValueDescription::typeCString(ValueType const a_type)
{
  auto typeID = static_cast<size_t>(a_type);
  assert(typeID < g_valuesDescriptions.size());

  return g_valuesDescriptions[typeID].typeString.c_str();
}

bool ValueDescription::isTypeAlowed(ValueType const a_type, IOSocketFlags::Flags const a_flags)
{
  auto typeID = static_cast<size_t>(a_type);
  assert(typeID < g_valuesDescriptions.size());

  return (1 << typeID) & a_flags;
}

ValueType ValueDescription::firstAvailableTypeForFlags(IOSocketFlags::Flags const a_flags)
{
  for (auto const &typeDescription : g_valuesDescriptions)
    if (a_flags & typeDescription.holdType) return typeDescription.type;

  assert(false);
}

std::pair<ElementColor, ElementColor> ValueDescription::typeColors(ValueType const a_type)
{
  auto typeID = static_cast<size_t>(a_type);
  assert(typeID < g_valuesDescriptions.size());

  return g_valuesDescriptions[typeID].colors;
}

ValueType ValueDescription::stringViewToType(std::string_view const a_type)
{
  for (auto const &typeDescription : g_valuesDescriptions)
    if (typeDescription.typeString == a_type) return typeDescription.type;

  assert(false);
}

std::vector<ValueType> ValueDescription::heldedValues(IOSocketFlags::Flags const a_flags)
{
  std::vector<ValueType> values{};

  for (auto const &typeDescription : g_valuesDescriptions)
    if (a_flags & typeDescription.holdType) values.push_back(typeDescription.type);

  assert(values.size());
  return values;
}

Value ValueDescription::defaultValue(ValueType const a_valueType)
{
  auto typeID = static_cast<size_t>(a_valueType);
  assert(typeID < g_valuesDescriptions.size());

  return g_valuesDescriptions[typeID].value;
}

template<typename T>
bool compare(Value const &a_a, Value const &a_b)
{
  return std::get<T>(a_a) == std::get<T>(a_b);
}

bool ValueDescription::compareValues(Value const &a_a, Value const &a_b, ValueType const a_type)
{
  switch (a_type) {
    case ValueType::eBool: return compare<bool>(a_a, a_b);
    case ValueType::eInt: return compare<int>(a_a, a_b);
    case ValueType::eFloat: return nearly_equal(std::get<double>(a_a), std::get<double>(a_b));
    case ValueType::eString: return compare<std::string>(a_a, a_b);
    case ValueType::eMatrix: assert(false);
    case ValueType::ePoint: return compare<cv::Point>(a_a, a_b);
    case ValueType::eShape: assert(false);
    case ValueType::eShapeVector: assert(false);
    default: assert(false);
  }

  return false;
}

} // namespace spaghetti
