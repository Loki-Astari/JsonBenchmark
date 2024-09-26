
#include "test.h"

#include "simdjson.h"
#include "TypeSafe.h"
#include <source_location>
#include "JsonifierCatalog.h"
#include "JsonifierCountry.h"
#include "JsonifierTwitter.h"

template<typename value_type>
concept bool_t = std::is_same_v<std::remove_cvref_t<value_type>, bool> || std::same_as<std::remove_cvref_t<value_type>, std::vector<bool>::reference> ||
std::same_as<std::remove_cvref_t<value_type>, std::vector<bool>::const_reference>;

template<typename value_type>
concept floating_type = std::floating_point<std::remove_cvref_t<value_type>> && !bool_t<value_type>;

template<typename value_type>
concept unsigned_type = std::unsigned_integral<std::remove_cvref_t<value_type>> && !floating_type<value_type> && !bool_t<value_type>;

template<typename value_type>
concept signed_type = std::signed_integral<std::remove_cvref_t<value_type>> && !floating_type<value_type> && !bool_t<value_type>;

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

template<typename value_type>
concept vector_subscriptable = requires(std::remove_cvref_t<value_type> value) {
	{ value[typename std::remove_cvref_t<value_type>::size_type{}] } -> std::same_as<typename std::remove_cvref_t<value_type>::const_reference>;
} || requires(std::remove_cvref_t<value_type> value) {
	{ value[typename std::remove_cvref_t<value_type>::size_type{}] } -> std::same_as<typename std::remove_cvref_t<value_type>::reference>;
};

template<typename value_type>
concept has_substr = requires(std::remove_cvref_t<value_type> value) {
	{
		value.substr(typename std::remove_cvref_t<value_type>::size_type{}, typename std::remove_cvref_t<value_type>::size_type{})
	} -> std::same_as<std::remove_cvref_t<value_type>>;
};

template<typename value_type>
concept has_resize = requires(std::remove_cvref_t<value_type> value) { value.resize(typename std::remove_cvref_t<value_type>::size_type{}); };

template<typename value_type>
concept has_data = requires(std::remove_cvref_t<value_type> value) {
	{ value.data() } -> std::same_as<typename std::remove_cvref_t<value_type>::const_pointer>;
} || requires(std::remove_cvref_t<value_type> value) {
	{ value.data() } -> std::same_as<typename std::remove_cvref_t<value_type>::pointer>;
};

template<typename value_type>
concept has_size = requires(std::remove_cvref_t<value_type> value) {
	{ value.size() } -> std::same_as<typename std::remove_cvref_t<value_type>::size_type>;
};

template<typename value_type>
concept string_t =
has_substr<value_type> && has_data<value_type> && has_size<value_type> && !std::is_same_v<std::remove_cvref_t<value_type>, char>&& vector_subscriptable<value_type>;

template<typename value_type>
concept copyable = std::copyable<std::remove_cvref_t<value_type>>;

template<typename value_type>
concept has_release = requires(std::remove_cvref_t<value_type> value) {
	{ value.release() } -> std::same_as<typename std::remove_cvref_t<value_type>::pointer>;
};

template<typename value_type>
concept has_get = requires(std::remove_cvref_t<value_type> value) {
	{ value.get() } -> std::same_as<typename std::remove_cvref_t<value_type>::element_type*>;
};

template<typename value_type>
concept unique_ptr_t = requires(std::remove_cvref_t<value_type> value) {
	typename std::remove_cvref_t<value_type>::element_type;
	typename std::remove_cvref_t<value_type>::deleter_type;
}&& has_release<value_type>;

template<typename value_type>
concept shared_ptr_t = has_get<value_type> && copyable<value_type>;

template<typename value_type>
concept vector_t = !map_t<value_type> && vector_subscriptable<value_type> && !std::is_pointer_v<std::remove_cvref_t<value_type>> && !string_t<value_type>;

void throwError(auto error, std::source_location location = std::source_location::current()) {
	std::stringstream stream{};
	stream << "Error: " << error << std::endl;
	stream << "Thrown from: " << location.file_name() << ", At Line: " << location.line() << std::endl;
	std::cout << stream.str();
	throw std::runtime_error{ stream.str() };
}

template<typename value_type> void getValue(value_type& data, simdjson::dom::element jsonData);

template<floating_type value_type> void getValue(value_type& data, simdjson::dom::element jsonData) {
	if constexpr (sizeof(value_type) == 8) {
		if (auto result = jsonData.get_double().get(data); result) {
			throwError(result);
		}
		return;
	}
	else {
		double newValue{};
		auto result = jsonData.get_double().get(newValue);
		if (result) {
			throwError(result);
		}
		data = static_cast<value_type>(newValue);
		return;
	}
}

template<unsigned_type value_type> void getValue(value_type& data, simdjson::dom::element jsonData) {
	uint64_t newValue{};
	auto result = jsonData.get_uint64().get(newValue);
	if (result) {
		throwError(result);
	}
	data = static_cast<value_type>(newValue);
	return;
}

template<signed_type value_type> void getValue(value_type& data, simdjson::dom::element jsonData) {
	int64_t newValue{};
	auto result = jsonData.get_int64().get(newValue);
	if (result) {
		throwError(result);
	}
	data = static_cast<value_type>(newValue);
	return;
}

template<> void getValue(std::nullptr_t&, simdjson::dom::element value) {
	auto result = value.is_null();
	if (!result) {
		throwError(result);
	}
}

template<> void getValue(std::string& data, simdjson::dom::element jsonData) {
	std::string_view newValue;
	auto result = jsonData.get(newValue);
	if (result) {
		throwError(result);
	}
	data = static_cast<std::string>(newValue);
}

template<bool_t value_type> void getValue(value_type& data, simdjson::dom::element jsonData) {
	bool newValue;
	auto result = jsonData.get(newValue);
	if (result) {
		throwError(result);
	}
	data = static_cast<value_type>(newValue);
	return;
}

template<vector_t value_type> void getValue(value_type& value, simdjson::dom::element jsonData) {
	auto newArr = jsonData.get_array();
	if (auto result = newArr.error()) {
		throwError(result);
	}
	auto newArray = newArr.value();
	const auto size = value.size();
	auto iter = newArray.begin();
	typename value_type::value_type valueNew;
	for (size_t x = 0; (x < size) && (iter != newArray.end()); ++x, ++iter) {
		getValue(valueNew, iter.operator*());
		value[x] = std::move(valueNew);
	}
	for (; iter != newArray.end(); ++iter) {
		getValue(valueNew, iter.operator*());
		value.emplace_back(std::move(valueNew));
	}
}

template<typename value_type> void getValue(std::optional<value_type>& vec, simdjson::dom::element jsonData) {
	if (!jsonData.is_null()) {
		getValue(vec.emplace(), jsonData);
	}
}

template<map_t value_type> void getValue(value_type& value, simdjson::dom::element jsonData) {
	simdjson::dom::object object;
	auto error = jsonData.get_object().get(object);
	if (error) {
		throwError(error);
	}
	for (auto field : object) {
		typename std::remove_cvref_t<decltype(value)>::key_type key;

		key = static_cast<typename std::remove_cvref_t<decltype(value)>::key_type>(std::string(field.key));
		typename std::remove_cvref_t<decltype(value)>::mapped_type newValue;
		getValue(newValue, field.value);
		value[key] = std::move(newValue);
	}

	return;
}

template<unique_ptr_t value_type> void getValue(value_type& e, simdjson::dom::element jsonData) {
	if (!e) {
		e = std::make_unique<std::remove_cvref_t<decltype(*e)>>();
	}
	return getValue(*e, jsonData);
}

template<shared_ptr_t value_type> void getValue(value_type& e, simdjson::dom::element jsonData) {
	if (!e) {
		e = std::make_shared<std::remove_cvref_t<decltype(*e)>>();
	}
	return getValue(*e, jsonData);
}

template<> void getValue(std::string*& value, simdjson::dom::element jsonData) {
	if (!value) {
		value = new std::string{};
	}
	return getValue(*value, jsonData);
}

template<typename value_type> void getValue(value_type& value, simdjson::dom::object jsonData, const std::string_view& key) {
	simdjson::dom::element jsonValue;
	auto error = jsonData[key].get(jsonValue);
	if (error == simdjson::SUCCESS) {
		return getValue(value, jsonValue);
	}
	else if (error == simdjson::NO_SUCH_FIELD) {
		return;
	}
	else {
		throwError(error);
	}
}

template<> void getValue(simdjson::dom::object& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	value = newObj.value();
}

template<> void getValue(hashtag_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.indices, obj, "indices");
	getValue(value.text, obj, "text");
}

template<> void getValue(large_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.w, obj, "w");
	getValue(value.h, obj, "h");
	getValue(value.resize, obj, "resize");
}

template<> void getValue(sizes_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.medium, obj, "medium");
	getValue(value.small, obj, "small");
	getValue(value.thumb, obj, "thumb");
	getValue(value.large, obj, "large");
}

template<> void getValue(media_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.source_status_id, obj, "source_status_id");
	getValue(value.source_status_id_str, obj, "source_status_id_str");
	getValue(value.id, obj, "id");
	getValue(value.id_str, obj, "id_str");
	getValue(value.indices, obj, "indices");
	getValue(value.media_url, obj, "media_url");
	getValue(value.media_url_https, obj, "media_url_https");
	getValue(value.url, obj, "url");
	getValue(value.display_url, obj, "display_url");
	getValue(value.expanded_url, obj, "expanded_url");
	getValue(value.type, obj, "type");
	getValue(value.sizes, obj, "sizes");
}

template<> void getValue(url_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.url, obj, "url");
	getValue(value.expanded_url, obj, "expanded_url");
	getValue(value.display_url, obj, "display_url");
	getValue(value.indices, obj, "indices");
}

template<> void getValue(user_mention& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.screen_name, obj, "screen_name");
	getValue(value.name, obj, "name");
	getValue(value.id, obj, "id");
	getValue(value.id_str, obj, "id_str");
	getValue(value.indices, obj, "indices");
}

template<> void getValue(status_entities& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.media, obj, "media");
	getValue(value.hashtags, obj, "hashtags");
	getValue(value.symbols, obj, "symbols");
	getValue(value.urls, obj, "urls");
	getValue(value.user_mentions, obj, "user_mentions");
}

template<> void getValue(metadata_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.result_type, obj, "result_type");
	getValue(value.iso_language_code, obj, "iso_language_code");
}

template<> void getValue(search_metadata_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.completed_in, obj, "completed_in");
	getValue(value.max_id, obj, "max_id");
	getValue(value.max_id_str, obj, "max_id_str");
	getValue(value.next_results, obj, "next_results");
	getValue(value.query, obj, "query");
	getValue(value.refresh_url, obj, "refresh_url");
	getValue(value.count, obj, "count");
	getValue(value.since_id, obj, "since_id");
	getValue(value.since_id_str, obj, "since_id_str");
}

template<> void getValue(description_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.urls, obj, "urls");
}

template<> void getValue(user_entities& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);
	getValue(value.url, obj, "url");
	getValue(value.description, obj, "description");
}

template<> void getValue(twitter_user_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(value.url, obj, "url");
	getValue(value.utc_offset, obj, "utc_offset");
	getValue(value.time_zone, obj, "time_zone");
	getValue(value.profile_banner_url, obj, "profile_banner_url");
	getValue(value.id, obj, "id");
	getValue(value.id_str, obj, "id_str");
	getValue(value.name, obj, "name");
	getValue(value.screen_name, obj, "screen_name");
	getValue(value.location, obj, "location");
	getValue(value.description, obj, "description");
	getValue(value.protectedVal, obj, "protected");
	getValue(value.followers_count, obj, "followers_count");
	getValue(value.friends_count, obj, "friends_count");
	getValue(value.listed_count, obj, "listed_count");
	getValue(value.created_at, obj, "created_at");
	getValue(value.favourites_count, obj, "favourites_count");
	getValue(value.geo_enabled, obj, "geo_enabled");
	getValue(value.verified, obj, "verified");
	getValue(value.statuses_count, obj, "statuses_count");
	getValue(value.lang, obj, "lang");
	getValue(value.contributors_enabled, obj, "contributors_enabled");
	getValue(value.is_translator, obj, "is_translator");
	getValue(value.is_translation_enabled, obj, "is_translation_enabled");
}

using tuple_type = Pass01;
static constexpr auto tupleSize = std::tuple_size_v<tuple_type>;

template<size_t currentIndex = 0> bool getValue(tuple_type& out_value, simdjson::dom::element json_value) {
	if constexpr (currentIndex < tupleSize) {
		getValue(std::get<currentIndex>(out_value), json_value);
		return getValue<currentIndex + 1>(out_value, json_value);
	}
	return true;
}

template<> void getValue(status_data& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(value.metadata, obj, "metadata");
	getValue(value.created_at, obj, "created_at");
	getValue(value.id, obj, "id");
	getValue(value.id_str, obj, "id_str");
	getValue(value.text, obj, "text");
	getValue(value.source, obj, "source");
	getValue(value.truncated, obj, "truncated");
	getValue(value.in_reply_to_status_id, obj, "in_reply_to_status_id");
	getValue(value.in_reply_to_status_id_str, obj, "in_reply_to_status_id_str");
	getValue(value.in_reply_to_user_id, obj, "in_reply_to_user_id");
	getValue(value.in_reply_to_user_id_str, obj, "in_reply_to_user_id_str");
	getValue(value.in_reply_to_screen_name, obj, "in_reply_to_screen_name");
	getValue(value.user, obj, "user");
	getValue(value.geo, obj, "geo");
	getValue(value.coordinates, obj, "coordinates");
	getValue(value.place, obj, "place");
	getValue(value.contributors, obj, "contributors");
	getValue(value.retweet_count, obj, "retweet_count");
	getValue(value.favorite_count, obj, "favorite_count");
	getValue(value.entities, obj, "entities");
	getValue(value.favorited, obj, "favorited");
	getValue(value.retweeted, obj, "retweeted");
	getValue(value.lang, obj, "lang");
	getValue(value.retweeted_status, obj, "retweeted_status");
	getValue(value.possibly_sensitive, obj, "possibly_sensitive");
}

template<> void getValue(twitter_message& value, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(value.statuses, obj, "statuses");
	getValue(value.search_metadata, obj, "search_metadata");
}

template<> void getValue(audience_sub_category_names& p, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(p.the337100890, obj, "337100890");
}

template<> void getValue(names&, simdjson::dom::element) {
	// No data extraction needed
}

template<> void getValue(event& e, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(e.description, obj, "description");
	getValue(e.id, obj, "id");
	getValue(e.logo, obj, "logo");
	getValue(e.name, obj, "name");
	getValue(e.topicIds, obj, "subTopicIds");
	getValue(e.subjectCode, obj, "subjectCode");
	getValue(e.subtitle, obj, "subtitle");
	getValue(e.topicIds, obj, "topicIds");
}

template<> void getValue(price& p, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(p.amount, obj, "amount");
	getValue(p.audienceSubCategoryId, obj, "audienceSubCategoryId");
	getValue(p.seatCategoryId, obj, "seatCategoryId");
}

template<> void getValue(area& a, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(a.areaId, obj, "areaId");
	getValue(a.blockIds, obj, "blockIds");
}

template<> void getValue(seat_category& sc, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(sc.areas, obj, "areas");
	getValue(sc.seatCategoryId, obj, "seatCategoryId");
}

template<> void getValue(venue_names& vn, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(vn.PLEYEL_PLEYEL, obj, "PLEYEL_PLEYEL");
}

template<> void getValue(performance& p, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(p.eventId, obj, "eventId");
	getValue(p.eventId, obj, "id");
	getValue(p.logo, obj, "logo");
	getValue(p.name, obj, "name");
	getValue(p.prices, obj, "prices");
	getValue(p.seatCategories, obj, "seatCategories");
	getValue(p.seatMapImage, obj, "seatMapImage");
	getValue(p.start, obj, "start");
	getValue(p.venueCode, obj, "venueCode");
}

template<> void getValue(citm_catalog_message& msg, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(msg.areaNames, obj, "areaNames");
	getValue(msg.audienceSubCategoryNames, obj, "audienceSubCategoryNames");
	getValue(msg.blockNames, obj, "blockNames");
	getValue(msg.events, obj, "events");
	getValue(msg.performances, obj, "performances");
	getValue(msg.seatCategoryNames, obj, "seatCategoryNames");
	getValue(msg.subTopicNames, obj, "subTopicNames");
	getValue(msg.subjectNames, obj, "subjectNames");
	getValue(msg.topicNames, obj, "topicNames");
	getValue(msg.topicSubTopics, obj, "topicSubTopics");
	getValue(msg.venueNames, obj, "venueNames");
}

template<> void getValue(geometry_data& geometry, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(geometry.type, obj, "type");
	getValue(geometry.coordinates, obj, "coordinates");
}

template<> void getValue(properties_data& properties, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(properties.name, obj, "name");
}

template<> void getValue(feature& f, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(f.type, obj, "type");
	getValue(f.properties, obj, "properties");
	getValue(f.geometry, obj, "geometry");
}

template<> void getValue(canada_message& message, simdjson::dom::element jsonData) {
	simdjson::dom::object obj{};
	getValue(obj, jsonData);

	getValue(message.type, obj, "type");
	getValue(message.features, obj, "features");
}

template<> void getValue(int*& out_value, simdjson::dom::element jsonData) {
	int64_t temp{};
	if (auto result = jsonData.get(temp); result) {
		throwError(result);
	}
	out_value = new int(static_cast<int>(temp));
}

template<> void getValue(std::unique_ptr<int>& out_value, simdjson::dom::element jsonData) {
	int64_t temp{};
	if (auto result = jsonData.get(temp); result) {
		throwError(result);
	}
	if (!out_value) {
		out_value = std::make_unique<int>(static_cast<int>(temp));
	}
	else {
		*out_value = static_cast<int>(temp);
	}
}

template<> void getValue(Empty&, simdjson::dom::element) {
	return ;
}

template<> void getValue(Special&, simdjson::dom::element) {
	return;
}

template<> void getValue(Obj2& message, simdjson::dom::element json_value) {
	return getValue(message.foo, json_value, "foo");
}

template<> void getValue(Obj3& message, simdjson::dom::element json_value) {
	getValue(message.a, json_value, "a");
	getValue(message.foo, json_value, "foo");
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
				return true;
			}
			return false;
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
				return true;
			}
			return false;
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
					streamDoc(child, s);
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
					s << sep << '"' << field.key << '"' << ':';
					streamDoc(field.value, s);
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
			std::unique_ptr<GetValueResult<value_type>> result = std::make_unique<GetValueResult<value_type>>();
			try
			{
				simdjson::padded_string jsonData{ json, length };
				if (auto resultNew = parser.parse(jsonData); !resultNew.error()) {
					result->doc = resultNew.value();
					getValue(result->data, result->doc);
					reply = std::move(result);
					return true;
				}
			}
			catch (...) {}
			std::cout << "Sorry, but parsing failed with simdjson-dom!" << std::endl;
			return true;
		}

		virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
		{
			return Prettify(parseResult, reply);
		}

		virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
		{
			GetValueResult<value_type> const& input = dynamic_cast<GetValueResult<value_type>const&>(parseResult);
			std::unique_ptr<StringResultUsingStream> result = std::make_unique<StringResultUsingStream>();
			streamDoc(input.doc, result->stream);
			reply = std::move(result);
			return true;
		}
	};

}