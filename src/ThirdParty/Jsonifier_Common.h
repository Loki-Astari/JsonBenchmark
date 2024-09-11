#ifndef JSONIFIERTYPES_H
#define JSONIFIERTYPES_H

#include "TypeSafe.h"
#include "jsonifier/Index.hpp"


template<> struct jsonifier::core<Obj3> {
	using value_type = Obj3;
	static constexpr auto parseValue = createValue<
		&value_type::a,
		&value_type::foo
	>();
};

template<> struct jsonifier::core<Obj2> {
	using value_type = Obj2;
	static constexpr auto parseValue = createValue<
		&value_type::foo
	>();
};

template<> struct jsonifier::core<Special> {
	using value_type = Special;
	static constexpr auto parseValue = createValue("integer", &value_type::integer, "real", &value_type::real, "E", &value_type::E, "zero", &value_type::zero, "one",
		&value_type::one, "space", &value_type::space, "quote", &value_type::quote, "backslash", &value_type::backslash, "controls", &value_type::controls, "slash",
		&value_type::slash, "alpha", &value_type::alpha, "ALPHA", &value_type::ALPHA, "0123456789", &value_type::digit, "number", &value_type::number, "special",
		&value_type::special, "hex", &value_type::hex, "null", &value_type::null, "array", &value_type::array);
};

template<> struct jsonifier::core<Empty> {
	using value_type = Empty;
	static constexpr auto parseValue = createValue();
};

namespace JsonifierTypes
{
	jsonifier::jsonifier_core<true> parser{};
	class VectorDouble : public TestAction
	{
	public:
		virtual bool ParseDouble(const char* json, long double& reply) const
		{
			std::vector<double> data{};
			if (parser.parseJson < jsonifier::parse_options{} > (data, jsonifier::string_view{ json })) {
				if (data.size() == 1) {
					reply = data[0];
				}
			}
			return true;
		}
	};

	class VectorString : public TestAction
	{
	public:
		virtual bool ParseString(const char* json, std::string& reply) const
		{
			std::vector<std::string> data{};
			if (parser.parseJson < jsonifier::parse_options{} > (data, jsonifier::string_view{ json })) {
				if (data.size() == 1) {
					reply = data[0];
				}
			}
			return true;
		}
	};
}

template<typename T>
struct GetValueResult: public ParseResultBase
{
    T       data;
};

template<typename T> struct jsonifier::core<GetValueResult<T>> {
	using value_type = GetValueResult<T>;
	static constexpr auto parseValue =
		createValue(&value_type::data);
};

namespace JsonifierTypes {

template<typename T>
class GetValue: public TestAction
{

  public:
	virtual bool Parse(const char* json, size_t, std::unique_ptr<ParseResultBase>& reply) const
	{
		std::unique_ptr<GetValueResult<T>>    parsedData = std::make_unique<GetValueResult<T>>();
		auto error = parser.parseJson(parsedData->data, json);
		if (error) [[likely]] {
			reply = std::move(parsedData);
			return true;
		}
        auto errors = parser.getErrors();
        for (const auto& error : errors) {
            std::cout << "Jsonifier Error: " << error.reportError() << "\n";
        }
		return true;
	}
	virtual bool Stringify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply)  const
	{
		return Prettify(parsedData, reply);
	}
	virtual bool Prettify(const ParseResultBase& parsedData, std::unique_ptr<StringResultBase>& reply) const
	{
		GetValueResult<T>const& parsedDataInput = dynamic_cast<GetValueResult<T> const&>(parsedData);
		std::unique_ptr<StringResultUsingString>    output = std::make_unique<StringResultUsingString>();
		auto error = parser.serializeJson < jsonifier::serialize_options{ .prettify = true } > (parsedDataInput.data, output->result);
		if (error) [[likely]] {
			reply = std::move(output);
		}
		return true;
	}
};

}

#endif
