#include <spaghetti/sockvalues.h>

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
    ValueType::eFloat,
    IOSocketFlags::eCanHoldFloat,
    "float",
    "Float",
    { { 254, 144, 50, 255 }, { 111, 80, 96, 255 } },
    0.0f
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
    ValueType::eMatrix,
    IOSocketFlags::eCanHoldMatrix,
    "matrix",
    "Matrix",
    { { 169, 169, 169, 255 }, { 169, 169, 169, 255 } },
    SafeValue<cv::Mat>()
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
    SafeValue<Shape>()
},
ValueTypeDescription{
    ValueType::eShapeVector,
    IOSocketFlags::eCanHoldShapeVector,
    "shape_vector",
    "ShapeVector",
    { { 207, 237, 57, 255 }, { 207, 237, 57, 255 } },
    SafeValue<ShapeVector>()
}
  // clang-format on
};

ValueTypeDescription::ValueTypeDescription(ValueType const a_type, IOSocketFlags::Flags const a_holdType,
                                           std::string const a_typeString, QString const a_typeQstring,
                                           std::pair<QColor, QColor> const a_colors, Value const a_value)
  : type{ a_type }
  , holdType{ a_holdType }
  , typeString{ a_typeString }
  , typeQstring{ a_typeQstring }
  , colors{ a_colors }
  , value{ a_value }
{
}

QString ValueDescription::typeQstring(ValueType const a_type)
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

std::pair<QColor, QColor> ValueDescription::typeColors(ValueType const a_type)
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

} // namespace spaghetti
