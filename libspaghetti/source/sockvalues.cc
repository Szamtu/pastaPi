#include <spaghetti/sockvalues.h>

namespace spaghetti {

QString ValueType_to_QString(ValueType const a_type)
{
  switch (a_type) {
    case ValueType::eBool: return "Bool";
    case ValueType::eInt: return "Int";
    case ValueType::eFloat: return "Float";
    case ValueType::eString: return "String";
    case ValueType::eMatrix: return "Matrix";
  }
  return "Unknown";
}

const char *getSocketType(ValueType const a_type)
{
  switch (a_type) {
    case ValueType::eBool: return "bool";
    case ValueType::eInt: return "int";
    case ValueType::eFloat: return "float";
    case ValueType::eString: return "string";
    case ValueType::eMatrix: return "matrix";
  }
  assert(false && "Wrong socket type");
  return "unknown";
}

bool value_type_allowed(uint8_t const a_flags, ValueType const a_type)
{
  switch (a_type) {
    case ValueType::eBool: return a_flags & IOSocketFlags::eCanHoldBool;
    case ValueType::eInt: return a_flags & IOSocketFlags::eCanHoldInt;
    case ValueType::eFloat: return a_flags & IOSocketFlags::eCanHoldFloat;
    case ValueType::eString: return a_flags & IOSocketFlags::eCanHoldString;
    case ValueType::eMatrix: return a_flags & IOSocketFlags::eCanHoldMatrix;
  }

  assert(false);
}

ValueType first_available_type_for_flags(uint8_t const a_flags)
{
  if (a_flags & IOSocketFlags::eCanHoldBool) return ValueType::eBool;
  if (a_flags & IOSocketFlags::eCanHoldInt) return ValueType::eInt;
  if (a_flags & IOSocketFlags::eCanHoldFloat) return ValueType::eFloat;
  if (a_flags & IOSocketFlags::eCanHoldString) return ValueType::eString;
  if (a_flags & IOSocketFlags::eCanHoldMatrix) return ValueType::eMatrix;

  assert(false);
}

std::pair<QColor, QColor> getTypeColor(ValueType const a_type)
{
  switch (a_type) {
    case ValueType::eBool: return { { 244, 53, 64, 255 }, { 75, 173, 88, 255 } };
    case ValueType::eInt: return { { 0, 170, 238, 255 }, { 0, 170, 238, 255 } };
    case ValueType::eFloat: return { { 254, 144, 50, 255 }, { 111, 80, 96, 255 } };
    case ValueType::eString: return { { 95, 124, 136, 255 }, { 95, 124, 136, 255 } };
    case ValueType::eMatrix: return { { 169, 169, 169, 255 }, { 169, 169, 169, 255 } };
  }

  assert(false);
}

ValueType stringViewToType(std::string_view const a_type)
{
  if (a_type == "bool")
    return ValueType::eBool;
  else if (a_type == "int")
    return ValueType::eInt;
  else if (a_type == "float")
    return ValueType::eFloat;
  else if (a_type == "string")
    return ValueType::eString;
  else if (a_type == "matrix")
    return ValueType::eMatrix;
  assert(false && "Wrong socket type");
  return ValueType::eBool;
}

std::vector<ValueType> getHoldedValues(IOSocketFlags::Flags const a_flags)
{
  std::vector<ValueType> values{};

  if (a_flags & IOSocketFlags::Flags::eCanHoldBool) values.push_back(ValueType::eBool);
  if (a_flags & IOSocketFlags::Flags::eCanHoldInt) values.push_back(ValueType::eInt);
  if (a_flags & IOSocketFlags::Flags::eCanHoldFloat) values.push_back(ValueType::eFloat);
  if (a_flags & IOSocketFlags::Flags::eCanHoldString) values.push_back(ValueType::eString);
  if (a_flags & IOSocketFlags::Flags::eCanHoldMatrix) values.push_back(ValueType::eMatrix);

  assert(values.size());

  return values;
}

Value getDefaultValue(ValueType const a_valueType)
{
  switch (a_valueType) {
    case ValueType::eBool: return false;
    case ValueType::eInt: return 0;
    case ValueType::eFloat: return 0.0f;
    case ValueType::eString: return std::string();
    case ValueType::eMatrix: return Matrix();
  }

  assert(false);
}

} // namespace spaghetti
