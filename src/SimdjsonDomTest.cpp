
#include "test.h"

#include "simdjson.h"
#include "TypeSafe.h"
#include "JsonifierCatalog.h"
#include "JsonifierCountry.h"
#include "JsonifierTwitter.h"

template<typename value_type>
concept bool_t = std::is_same_v<std::remove_cvref_t<value_type>, bool> || std::same_as<std::remove_cvref_t<value_type>, std::vector<bool>::reference> ||
std::same_as<std::remove_cvref_t<value_type>, std::vector<bool>::const_reference>;

template<typename value_type>
concept signed_type = std::signed_integral<std::remove_cvref_t<value_type>> && !bool_t<value_type>;

template<typename value_type>
concept unsigned_type = std::unsigned_integral<std::remove_cvref_t<value_type>> && !bool_t<value_type>;

template<signed_type value_type> bool getValue(value_type& data, simdjson::dom::element json_value) {
	int64_t newValue{};
	auto result = json_value.get_int64().get(newValue);
	data = static_cast<value_type>(newValue);
	return !result;
}

template<unsigned_type value_type> bool getValue(value_type& data, simdjson::dom::element json_value) {
	uint64_t newValue{};
	auto result = json_value.get_uint64().get(newValue);
	data = static_cast<value_type>(newValue);
	return !result;
}

template<> inline bool getValue(long& data, simdjson::dom::element json_value) {
	int64_t newValue{};
	auto result = json_value.get_int64().get(newValue);
	data = static_cast<long>(newValue);
	return !result;
}

template<> inline bool getValue(int& data, simdjson::dom::element json_value) {
	uint64_t newValue{};
	auto result = json_value.get_uint64().get(newValue);
	data = static_cast<int>(newValue);
	return !result;
}

template<typename value_type>
concept range = requires(std::remove_cvref_t<value_type> value) {
	typename std::remove_cvref_t<value_type>::value_type;
	{ value.begin() } -> std::same_as<typename std::remove_cvref_t<value_type>::const_iterator>;
	{ value.end() } -> std::same_as<typename std::remove_cvref_t<value_type>::const_iterator>;
} || requires(std::remove_cvref_t<value_type> value) {
	typename std::remove_cvref_t<value_type>::value_type;
	{ value.begin() } -> std::same_as<typename std::remove_cvref_t<value_type>::iterator>;
	{ value.end() } -> std::same_as<typename std::remove_cvref_t<value_type>::iterator>;
};

template<typename value_type>
concept map_subscriptable = requires(std::remove_cvref_t<value_type> value) {
	{ value[typename std::remove_cvref_t<value_type>::key_type{}] } -> std::same_as<const typename std::remove_cvref_t<value_type>::mapped_type&>;
} || requires(std::remove_cvref_t<value_type> value) {
	{ value[typename std::remove_cvref_t<value_type>::key_type{}] } -> std::same_as<typename std::remove_cvref_t<value_type>::mapped_type&>;
};

template<typename value_type>
concept map_t = requires(std::remove_cvref_t<value_type> value) {
	typename std::remove_cvref_t<value_type>::mapped_type;
	typename std::remove_cvref_t<value_type>::key_type;
}&& range<value_type>&& map_subscriptable<value_type> && !std::is_integral_v<std::remove_cvref_t<value_type>>;

inline std::string to_string(std::string_view view) {
	return std::string(view.data(), view.size());
}

template<typename value_type> bool getValue(value_type& value, simdjson::dom::element json_value);

template<> bool getValue(std::string& data, simdjson::dom::element json_value) {
	std::string_view val{};
	auto error = json_value.get(val);
	if (error)
		return false;
	data = val;
	return true;
}

template<> bool getValue(double& data, simdjson::dom::element json_value) {
	return !json_value.get_double().get(data);
}

template<> bool getValue(uint64_t& data, simdjson::dom::element json_value) {
	auto result = json_value.get_uint64().get(data);
	return !result;
}

template<> bool getValue(bool& data, simdjson::dom::element json_value) {
	return !json_value.get_bool().get(data);
}

template<bool_t value_type> bool getValue(value_type&& data, simdjson::dom::element json_value) {
	bool val{};
	if (json_value.get_bool().get(val))
		return false;
	data = val;
	return true;
}

template<typename value_type> bool getValue(std::vector<value_type>& value, simdjson::dom::element json_value) {
	auto array = json_value.get_array();
	for (auto element : array) {
		value_type newValue;
		if (!getValue(newValue, element))
			return false;
		value.push_back(newValue);
	}
	return true;
}

template<typename value_type> bool getValue(std::optional<value_type>& vec, simdjson::dom::element json_value) {
	value_type newValue;
	if (!getValue(newValue, json_value))
		return false;
	vec = newValue;
	return true;
}

template<> bool getValue(std::vector<bool>& value, simdjson::dom::element json_value) {
	auto array = json_value.get_array();
	for (auto element : array) {
		bool newValue;
		if (!getValue(newValue, element))
			return false;
		value.push_back(newValue);
	}
	return true;
}

template<> bool getValue(std::vector<std::nullptr_t>&, simdjson::dom::element json_value) {
	auto array = json_value.get_array();
	for (auto element : array) {
		if (!element.is_null())
			return false;
	}
	return true;
}

template<> bool getValue(std::string*& value, simdjson::dom::element json_value) {
	if (!value) {
		value = new std::string{};
	}
	return getValue(*value, json_value);
}

template<map_t map_type> bool getValue(map_type& map, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	for (auto [key_view, element] : obj) {
		std::string key = std::string(key_view);
		typename map_type::mapped_type newValue;
		if (!getValue(newValue, element)) {
			return false;
		}
		map[key] = newValue;
	}
	return true;
}

template<> bool getValue(std::nullptr_t&, simdjson::dom::element value) {
	return value.is_null();
}

template<>
bool getValue(hashtag_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.text, obj["text"])) {
		return false;
	}
	return getValue(value.indices, obj["indices"]);
}

template<>
bool getValue(large_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.w, obj["w"]) ||
		!getValue(value.h, obj["h"]) ||
		!getValue(value.resize, obj["resize"])) {
		return false;
	}
	return true;
}

template<>
bool getValue(sizes_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(value.medium, obj["medium"]) &&
		getValue(value.small, obj["small"]) &&
		getValue(value.thumb, obj["thumb"]) &&
		getValue(value.large, obj["large"]);
}

template<>
bool getValue(media_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.id, obj["id"]) ||
		!getValue(value.id_str, obj["id_str"]) ||
		!getValue(value.indices, obj["indices"]) ||
		!getValue(value.media_url, obj["media_url"]) ||
		!getValue(value.media_url_https, obj["media_url_https"]) ||
		!getValue(value.url, obj["url"]) ||
		!getValue(value.display_url, obj["display_url"]) ||
		!getValue(value.expanded_url, obj["expanded_url"]) ||
		!getValue(value.type, obj["type"]) ||
		!getValue(value.sizes, obj["sizes"]) ||
		!getValue(value.source_status_id, obj["source_status_id"]) ||
		!getValue(value.source_status_id_str, obj["source_status_id_str"])) {
		return false;
	}
	return true;
}

template<>
bool getValue(url_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.url, obj["url"]) ||
		!getValue(value.expanded_url, obj["expanded_url"]) ||
		!getValue(value.display_url, obj["display_url"]) ||
		!getValue(value.indices, obj["indices"])) {
		return false;
	}
	return true;
}

template<>
bool getValue(user_mention& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.screen_name, obj["screen_name"]) ||
		!getValue(value.name, obj["name"]) ||
		!getValue(value.id, obj["id"]) ||
		!getValue(value.id_str, obj["id_str"]) ||
		!getValue(value.indices, obj["indices"])) {
		return false;
	}
	return true;
}

template<> bool getValue(status_entities& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(value.hashtags, obj["hashtags"]) &&
		getValue(value.symbols, obj["symbols"]) &&
		getValue(value.urls, obj["urls"]) &&
		getValue(value.user_mentions, obj["user_mentions"]) &&
		getValue(value.media, obj["media"]);
}

template<> bool getValue(metadata_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.result_type, obj["result_type"]) ||
		!getValue(value.iso_language_code, obj["iso_language_code"])) {
		return false;
	}

	return true;
}

template<>
bool getValue(search_metadata_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.completed_in, obj["completed_in"])) {
		return false;
	}
	if (!getValue(value.max_id, obj["max_id"])) {
		return false;
	}
	if (!getValue(value.max_id_str, obj["max_id_str"])) {
		return false;
	}
	if (!getValue(value.next_results, obj["next_results"])) {
		return false;
	}
	if (!getValue(value.query, obj["query"])) {
		return false;
	}
	if (!getValue(value.refresh_url, obj["refresh_url"])) {
		return false;
	}
	if (!getValue(value.count, obj["count"])) {
		return false;
	}
	if (!getValue(value.since_id, obj["since_id"])) {
		return false;
	}
	if (!getValue(value.since_id_str, obj["since_id_str"])) {
		return false;
	}
	return true;
}

template<> bool getValue(twitter_user_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.id, obj["id"]) ||
		!getValue(value.id_str, obj["id_str"]) ||
		!getValue(value.name, obj["name"]) ||
		!getValue(value.screen_name, obj["screen_name"]) ||
		!getValue(value.location, obj["location"]) ||
		!getValue(value.description, obj["description"]) ||
		!getValue(value.url, obj["url"]) ||
		!getValue(value.protectedVal, obj["protected"]) ||
		!getValue(value.followers_count, obj["followers_count"]) ||
		!getValue(value.friends_count, obj["friends_count"]) ||
		!getValue(value.listed_count, obj["listed_count"]) ||
		!getValue(value.created_at, obj["created_at"]) ||
		!getValue(value.favourites_count, obj["favourites_count"]) ||
		!getValue(value.utc_offset, obj["utc_offset"]) ||
		!getValue(value.time_zone, obj["time_zone"]) ||
		!getValue(value.geo_enabled, obj["geo_enabled"]) ||
		!getValue(value.verified, obj["verified"]) ||
		!getValue(value.statuses_count, obj["statuses_count"]) ||
		!getValue(value.lang, obj["lang"]) ||
		!getValue(value.is_translator, obj["is_translator"]) ||
		!getValue(value.is_translation_enabled, obj["is_translation_enabled"])) {
		return false;
	}

	return true;
}

template<typename value_type>
bool getValue(value_type&, simdjson::dom::element) {
	return false;
}

template<typename value_type>
bool getValue(std::unique_ptr<value_type>& e, simdjson::dom::element json_value) {
	if (!e) e = std::make_unique<value_type>();
	return getValue(*e, json_value);
}

template<typename value_type>
bool getValue(std::shared_ptr<value_type>& e, simdjson::dom::element json_value) {
	if (!e) e = std::make_shared<value_type>();
	return getValue(*e, json_value);
}

template<>
bool getValue(status_data& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	if (!getValue(value.metadata, obj["metadata"]) ||
		!getValue(value.created_at, obj["created_at"]) ||
		!getValue(value.id, obj["id"]) ||
		!getValue(value.id_str, obj["id_str"]) ||
		!getValue(value.text, obj["text"]) ||
		!getValue(value.source, obj["source"]) ||
		!getValue(value.truncated, obj["truncated"]) ||
		!getValue(value.in_reply_to_status_id, obj["in_reply_to_status_id"]) ||
		!getValue(value.in_reply_to_status_id_str, obj["in_reply_to_status_id_str"]) ||
		!getValue(value.in_reply_to_user_id, obj["in_reply_to_user_id"]) ||
		!getValue(value.in_reply_to_user_id_str, obj["in_reply_to_user_id_str"]) ||
		!getValue(value.in_reply_to_screen_name, obj["in_reply_to_screen_name"]) ||
		!getValue(value.user, obj["user"]) ||
		!getValue(value.geo, obj["geo"]) ||
		!getValue(value.coordinates, obj["coordinates"]) ||
		!getValue(value.place, obj["place"]) ||
		!getValue(value.contributors, obj["contributors"]) ||
		!getValue(value.retweet_count, obj["retweet_count"]) ||
		!getValue(value.favorite_count, obj["favorite_count"]) ||
		!getValue(value.entities, obj["entities"]) ||
		!getValue(value.favorited, obj["favorited"]) ||
		!getValue(value.retweeted, obj["retweeted"]) ||
		!getValue(value.lang, obj["lang"]) ||
		!getValue(value.retweeted_status, obj["retweeted_status"]) ||
		!getValue(value.possibly_sensitive, obj["possibly_sensitive"])) {
		return false;
	}

	return true;
}

template<>
bool getValue(twitter_message& value, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(value.statuses, obj["statuses"]) &&
		getValue(value.search_metadata, obj["search_metadata"]);
}

template<>
bool getValue(audience_sub_category_names& p, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(p.the337100890, obj["337100890"]);
}

template<>
bool getValue(price& p, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(p.amount, obj["amount"]) &&
		getValue(p.audienceSubCategoryId, obj["audienceSubCategoryId"]) &&
		getValue(p.seatCategoryId, obj["seatCategoryId"]);
}

template<>
bool getValue(area& a, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(a.areaId, obj["areaId"]) &&
		getValue(a.blockIds, obj["blockIds"]);
}

template<>
bool getValue(seat_category& sc, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(sc.areas, obj["areas"]) &&
		getValue(sc.seatCategoryId, obj["seatCategoryId"]);
}

template<>
bool getValue(venue_names& vn, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(vn.PLEYEL_PLEYEL, obj["PLEYEL_PLEYEL"]);
}

template<>
bool getValue(names&, simdjson::dom::element) {
	return true;
}

template<>
bool getValue(event& e, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(e.description, obj["description"]) &&
		getValue(e.id, obj["id"]) &&
		getValue(e.logo, obj["logo"]) &&
		getValue(e.name, obj["name"]) &&
		getValue(e.subTopicIds, obj["subTopicIds"]) &&
		getValue(e.subjectCode, obj["subjectCode"]) &&
		getValue(e.subtitle, obj["subtitle"]) &&
		getValue(e.topicIds, obj["topicIds"]);
}

template<>
bool getValue(performance& p, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(p.eventId, obj["eventId"]) &&
		getValue(p.id, obj["id"]) &&
		getValue(p.logo, obj["logo"]) &&
		getValue(p.name, obj["name"]) &&
		getValue(p.prices, obj["prices"]) &&
		getValue(p.seatCategories, obj["seatCategories"]) &&
		getValue(p.venueCode, obj["venueId"]);
}

using tuple_type = Pass01;

template<>
bool getValue(tuple_type& t, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(std::get<0>(t), obj["first_element"]) &&
		getValue(std::get<1>(t), obj["second_element"]);
}

template<>
bool getValue(citm_catalog_message& msg, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error() != simdjson::SUCCESS) return false;
	if (!getValue(msg.areaNames, obj["areaNames"])) return false;
	if (!getValue(msg.audienceSubCategoryNames, obj["audienceSubCategoryNames"])) return false;
	if (!getValue(msg.blockNames, obj["blockNames"])) return false;
	if (!getValue(msg.events, obj["events"])) return false;
	if (!getValue(msg.performances, obj["performances"])) return false;
	if (!getValue(msg.seatCategoryNames, obj["seatCategoryNames"])) return false;
	if (!getValue(msg.subTopicNames, obj["subTopicNames"])) return false;
	if (!getValue(msg.subjectNames, obj["subjectNames"])) return false;
	if (!getValue(msg.topicNames, obj["topicNames"])) return false;
	if (!getValue(msg.topicSubTopics, obj["topicSubTopics"])) return false;
	if (!getValue(msg.venueNames, obj["venueNames"])) return false;

	return true;
}

template<>
bool getValue(geometry_data& geometry, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(geometry.type, obj["type"]) &&
		getValue(geometry.coordinates, obj["coordinates"]);
}

template<>
bool getValue(properties_data& properties, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(properties.name, obj["name"]);
}

template<>
bool getValue(feature& f, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(f.type, obj["type"]) &&
		getValue(f.properties, obj["properties"]) &&
		getValue(f.geometry, obj["geometry"]);
}

template<>
bool getValue(canada_message& message, simdjson::dom::element json_value) {
	auto obj = json_value.get_object();
	if (obj.error()) return false;

	return getValue(message.type, obj["type"]) &&
		getValue(message.features, obj["features"]);
}

template<>
bool getValue(int*& out_value, simdjson::dom::element json_value) {
	int64_t temp{};
	if (json_value.get(temp) != simdjson::SUCCESS) {
		return false;
	}
	out_value = new int(static_cast<int>(temp));
	return true;
}

template<> bool getValue(std::unique_ptr<int>& out_value, simdjson::dom::element json_value) {
	int64_t temp{};
	if (json_value.get(temp) != simdjson::SUCCESS) {
		return false;
	}
	out_value = std::make_unique<int>(static_cast<int>(temp));
	return true;
}

template<>
bool getValue(Obj2& message, simdjson::dom::element json_value) {
	return getValue(message.foo, json_value["foo"]);
}

template<>
bool getValue(Obj3& message, simdjson::dom::element json_value) {
	return getValue(message.a, json_value["a"]) &&
		getValue(message.foo, json_value["foo"]);
}

namespace SimdjsonDomTypes
{
	simdjson::dom::parser parser{};
	class VectorDouble : public TestAction
	{
	public:
		virtual bool ParseDouble(const char* json, long double& reply) const
		{
			simdjson::dom::element      doc;
			simdjson::padded_string jsonStr(json, strlen(json));
			if (parser.parse(jsonStr).get(doc) != simdjson::SUCCESS) {
				return true;
			}
			std::vector<double> data;
			getValue(data, doc);
			if (data.size() == 1) {
				reply = data[0];
			}
			return true;
		}
	};

	class VectorString : public TestAction
	{
	public:
		virtual bool ParseString(const char* json, std::string& reply) const
		{
			simdjson::dom::element doc;
			simdjson::padded_string jsonStr(json, strlen(json));
			if (parser.parse(jsonStr).get(doc) != simdjson::SUCCESS) {
				return true;
			}
			std::vector<std::string> data;
			getValue(data, doc);
			if (data.size() == 1) {
				reply = data[0];
			}
			return true;
		}
	};
}

template<typename value_type>
struct GetValueResult : public ParseResultBase
{
	value_type       data;
	simdjson::dom::element doc;
};

template<typename value_type>
bool getValue(GetValueResult<value_type>& f, simdjson::dom::element json_value) {
	return getValue(f.data, json_value);
}

namespace SimdjsonDomTypes {

	template<typename value_type>
	class GetValue : public TestAction
	{

	public:

		void streamDoc(simdjson::dom::element element, std::ostream& s) const
		{
			switch (element.type())
			{
			case simdjson::dom::element_type::ARRAY:
			{
				char sep = ' ';
				s << '[';
				for (auto child : element.get_array()) {
					s << sep;
					streamDoc(child.value(), s);
					sep = ',';
				}
				s << ']';
				break;
			}
			case simdjson::dom::element_type::OBJECT:
			{
				char sep = ' ';
				s << '{';
				for (auto field : element.get_object()) {
					s << sep << '"' << field.key() << '"' << ':';
					streamDoc(field.value(), s);
					sep = ',';
				}
				s << '}';
				break;
			}
			case simdjson::dom::element_type::DOUBLE:
			case simdjson::dom::element_type::UINT64:
			case simdjson::dom::element_type::INT64:
			{
				double value = element.get_double();
				s << std::setprecision(17) << value;
				break;
			}
			case simdjson::dom::element_type::STRING:
			{
				std::string_view value = element.get_string();
				s << '"' << value << '"';
				break;
			}
			case simdjson::dom::element_type::BOOL:
			{
				bool    value = element.get_bool();
				s << (value ? "true" : "false");
				break;
			}
			case simdjson::dom::element_type::NULL_VALUE:
			{
				s << "null";
				break;
			}
			default: {
				throw std::runtime_error("Fail");
			}
			}
		}

		virtual bool Parse(const char* json, size_t length, std::unique_ptr<ParseResultBase>& reply) const override
		{
			std::unique_ptr<GetValueResult<value_type>> result = std::make_unique<GetValueResult<value_type>>(json, length);
			try
			{
				simdjson::padded_string jsonData{ json, length };
				if (const auto resultNew = parser.parse(jsonData); !resultNew.error()) {
					result->doc = *resultNew.begin();
					if (!getValue(result->data, result->doc)) {
						reply = std::move(result);
						return true;
					}
				}
				std::cout << "Sorry, but parsing failed with simdjson-dom!" << std::endl;
			}
			catch (...) {}
			return true;
		}

		virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
		{
			return Prettify(parseResult, reply);
		}

		virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
		{
			GetValueResult<value_type> const& input = dynamic_cast<GetValueResult<value_type>const&>(parseResult);
			auto& const val = input.doc;
			std::unique_ptr<StringResultUsingStream> result = std::make_unique<StringResultUsingStream>();
			streamDoc(val, result->stream);
			reply = std::move(result);
			return true;
		}
	};

}

// OK: The TypeSafeTest test is a template type that has a handler for each test case.
//     There are three default handlers that you need to provide.
//          class VectorDouble: public TestAction
//          class VectorString: public TestAction
//          template<typename T>
//          class GetValue: public TestAction
//
// See the file: SimdjsonDomTypes.h for the framework.
//
// This should cover most cases.
// If you need to do some special handling of types you can override the default actions.
// See comments blow:

class SimdjsonDomTest : public TypeSafeTest<SimdjsonDomTypes::VectorDouble,      // Type for parsing array of single double.
	SimdjsonDomTypes::VectorString,      // Type ofr parsing array of single string
	SimdjsonDomTypes::GetValue>          // Template template class. This is templatezed with the type
	// That needs to be read from the json string.
{
	SimdjsonDomTypes::GetValue<canada_message>                    testGetValueCountry;
	SimdjsonDomTypes::GetValue<twitter_message>                   testGetValueTwitter;
	SimdjsonDomTypes::GetValue<citm_catalog_message>              testGetValueCatalog;
public:
	SimdjsonDomTest()
	{
		actionMap["performance/canada.json"] = &testGetValueCountry;
		actionMap["performance/twitter.json"] = &testGetValueTwitter;
		actionMap["performance/citm_catalog.json"] = &testGetValueCatalog;
	}

	virtual const char* GetName()     const override { return "SimdjsonDom"; }
	virtual const char* Type()        const override { return "C++"; }
	virtual const char* GetFilename() const override { return __FILE__; }
};

REGISTER_TEST(SimdjsonDomTest);