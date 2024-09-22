
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

template<typename value_type> concept string_t = std::convertible_to<std::remove_cvref_t<value_type>, std::string> || std::same_as<std::remove_cvref_t<value_type>, std::string> || std::same_as<std::remove_cvref_t<value_type>, std::string_view>;

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

template<typename value_type> bool getValue(value_type& data, simdjson::dom::element jsonData);

template<floating_type value_type> bool getValue(value_type& data, simdjson::dom::element jsonData) {
	double newValue{};
	auto result = jsonData.get_double().get(newValue);
	if (result) {
		std::cout << "CURRENT ERROR: " << result << std::endl;
		throw std::runtime_error{ "FAILED" };
	}
	data = static_cast<value_type>(newValue);
	return !result;
}

template<unsigned_type value_type>
bool getValue(value_type& data, simdjson::dom::element jsonData) {
	uint64_t newValue{};
	auto result = jsonData.get_uint64().get(newValue);
	if (result) {
		std::cout << "CURRENT ERROR: " << result << std::endl;
		throw std::runtime_error{ "FAILED" };
	}
	data = static_cast<value_type>(newValue);
	return !result;
}

template<signed_type value_type> bool getValue(value_type& data, simdjson::dom::element jsonData) {
	int64_t newValue{};
	auto result = jsonData.get_int64().get(newValue);
	if (result) {
		std::cout << "CURRENT ERROR: " << result << std::endl;
		throw std::runtime_error{ "FAILED" };
	}
	data = static_cast<value_type>(newValue);
	return !result;
}

template<> bool getValue(std::nullptr_t&, simdjson::dom::element value) {
	return value.is_null();
}

template<> bool getValue(std::string& data, simdjson::dom::element jsonData) {
	std::string_view newValue;
	auto result = jsonData.get(newValue);
	if (result) {
		std::cout << "CURRENT ERROR: " << result << std::endl;
		throw std::runtime_error{ "FAILED" };
	}
	data = static_cast<std::string>(newValue);
	return !result;
}

template<bool_t value_type> bool getValue(value_type& data, simdjson::dom::element jsonData) {
	bool newValue;
	auto result = jsonData.get(newValue);
	if (result) {
		std::cout << "CURRENT ERROR: " << result << std::endl;
		throw std::runtime_error{ "FAILED" };
	}
	data = static_cast<value_type>(newValue);
	return !result;
}

template<> bool getValue(int*& out_value, simdjson::dom::element jsonData) {
	int64_t temp{};
	if (jsonData.get(temp)) {
		return false;
	}
	out_value = new int(static_cast<int>(temp));
	return true;
}

template<> bool getValue(std::unique_ptr<int>& out_value, simdjson::dom::element jsonData) {
	int64_t temp;
	if (jsonData.get(temp)) {
		std::cerr << "Error parsing integer value\n";
		return false;
	}
	out_value = std::make_unique<int>(temp);
	return true;
}

template<vector_t value_type> bool getValue(value_type& value, simdjson::dom::element jsonData) {
	const auto size = value.size();
	auto newArray = jsonData.get_array();
	auto iter = newArray.begin();
	typename value_type::value_type valueNew;
	for (size_t x = 0; (x < size) && (iter != newArray.end()); ++x, ++iter) {
		if (!getValue(valueNew, *iter)) {
			return false;
		}
		value[x] = std::move(valueNew);
	}
	for (; iter != newArray.end(); ++iter) {
		if (!getValue(valueNew, *iter)) {
			return false;
		}
		value.emplace_back(std::move(valueNew));
	}
	return true;
}

template<typename value_type> bool getValue(std::optional<value_type>& vec, simdjson::dom::element jsonData) {
	if (!jsonData.is_null()) {
		if (!getValue(vec.emplace(), jsonData)) {
			return false;
		}
	}
	return true;
}

template<map_t value_type> bool getValue(value_type& value, simdjson::dom::element jsonData) {
	simdjson::dom::object object;
	auto error = jsonData.get_object().get(object);
	if (error) {
		return false;
	}
	for (auto field : object) {

		auto key = static_cast<typename std::remove_cvref_t<decltype(value)>::key_type>(std::string(field.key));

		simdjson::dom::element field_value = field.value;
		typename std::remove_cvref_t<decltype(value)>::mapped_type newValue;
		if (!getValue(newValue, field_value)) {
			return false;
		}
		value[key] = std::move(newValue);
	}

	return true;
}

template<> bool getValue(std::string*& value, simdjson::dom::element jsonData) {
	if (!value) {
		value = new std::string{};
	}
	return getValue(*value, jsonData);
}

template<typename value_type> bool getValue(value_type& value, simdjson::dom::object jsonData, const std::string_view& key) {
	simdjson::dom::element jsonValue;
	auto error = jsonData[key].get(jsonValue);
	if (error == simdjson::SUCCESS) {
		return getValue(value, jsonValue);
	}
	else if (error == simdjson::NO_SUCH_FIELD) {
		return true;
	}
	else {
		return false;
	}
}

template<> bool getValue(hashtag_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	return getValue(value.indices, obj, "indices") && getValue(value.text, obj, "text");
}

template<> bool getValue(large_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	return getValue(value.w, obj, "w") && getValue(value.h, obj, "h") && getValue(value.resize, obj, "resize");
}

template<> bool getValue(sizes_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	return getValue(value.medium, obj, "medium") && getValue(value.small, obj, "small") && getValue(value.thumb, obj, "thumb") && getValue(value.large, obj, "large");
}

template<> bool getValue(media_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	if (!getValue(value.source_status_id, obj, "source_status_id"))
		return false;
	if (!getValue(value.source_status_id_str, obj, "source_status_id_str"))
		return false;
	if (!getValue(value.id, obj, "id"))
		return false;
	if (!getValue(value.id_str, obj, "id_str"))
		return false;
	if (!getValue(value.indices, obj, "indices"))
		return false;
	if (!getValue(value.media_url, obj, "media_url"))
		return false;
	if (!getValue(value.media_url_https, obj, "media_url_https"))
		return false;
	if (!getValue(value.url, obj, "url"))
		return false;
	if (!getValue(value.display_url, obj, "display_url"))
		return false;
	if (!getValue(value.expanded_url, obj, "expanded_url"))
		return false;
	if (!getValue(value.type, obj, "type"))
		return false;
	if (!getValue(value.sizes, obj, "sizes"))
		return false;
	return true;
}

template<> bool getValue(url_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	if (!getValue(value.url, obj, "url"))
		return false;
	if (!getValue(value.expanded_url, obj, "expanded_url"))
		return false;
	if (!getValue(value.display_url, obj, "display_url"))
		return false;
	if (!getValue(value.indices, obj, "indices"))
		return false;

	return true;
}

template<> bool getValue(user_mention& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	if (!getValue(value.screen_name, obj, "screen_name"))
		return false;
	if (!getValue(value.name, obj, "name"))
		return false;
	if (!getValue(value.id, obj, "id"))
		return false;
	if (!getValue(value.id_str, obj, "id_str"))
		return false;

	return getValue(value.indices, obj, "indices");
}

template<> bool getValue(status_entities& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	if (!getValue(value.media, obj, "media")) {
		return false;
	}
	return getValue(value.hashtags, obj, "hashtags") && getValue(value.symbols, obj, "symbols") && getValue(value.urls, obj, "urls") &&
		getValue(value.user_mentions, obj, "user_mentions");
}

template<> bool getValue(metadata_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	if (!getValue(value.result_type, obj, "result_type"))
		return false;
	if (!getValue(value.iso_language_code, obj, "iso_language_code"))
		return false;

	return true;
}

template<> bool getValue(search_metadata_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	if (!getValue(value.completed_in, obj, "completed_in"))
		return false;
	if (!getValue(value.max_id, obj, "max_id"))
		return false;
	if (!getValue(value.max_id_str, obj, "max_id_str"))
		return false;
	if (!getValue(value.next_results, obj, "next_results"))
		return false;
	if (!getValue(value.query, obj, "query"))
		return false;
	if (!getValue(value.refresh_url, obj, "refresh_url"))
		return false;
	if (!getValue(value.count, obj, "count"))
		return false;
	if (!getValue(value.since_id, obj, "since_id"))
		return false;
	if (!getValue(value.since_id_str, obj, "since_id_str"))
		return false;

	return true;
}

template<> bool getValue(description_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	return getValue(value.urls, obj, "urls");
}

template<> bool getValue(user_entities& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	if (!getValue(value.url, obj, "url")) {
		return false;
	}
	return getValue(value.description, obj, "description");
}

template<> bool getValue(twitter_user_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	if (!getValue(value.url, obj, "url")) {
		return false;
	}
	if (!getValue(value.utc_offset, obj, "utc_offset")) {
		return false;
	}
	if (!getValue(value.time_zone, obj, "time_zone")) {
		return false;
	}
	if (!getValue(value.profile_banner_url, obj, "profile_banner_url")) {
		return false;
	}
	if (!getValue(value.id, obj, "id") || !getValue(value.id_str, obj, "id_str") || !getValue(value.name, obj, "name") || !getValue(value.screen_name, obj, "screen_name") ||
		!getValue(value.location, obj, "location") || !getValue(value.description, obj, "description") || !getValue(value.protectedVal, obj, "protected") ||
		!getValue(value.followers_count, obj, "followers_count") || !getValue(value.friends_count, obj, "friends_count") || !getValue(value.listed_count, obj, "listed_count") ||
		!getValue(value.created_at, obj, "created_at") || !getValue(value.favourites_count, obj, "favourites_count") || !getValue(value.geo_enabled, obj, "geo_enabled") ||
		!getValue(value.verified, obj, "verified") || !getValue(value.statuses_count, obj, "statuses_count") || !getValue(value.lang, obj, "lang") ||
		!getValue(value.contributors_enabled, obj, "contributors_enabled") || !getValue(value.is_translator, obj, "is_translator") ||
		!getValue(value.is_translation_enabled, obj, "is_translation_enabled")) {
		return false;
	}

	return true;
}

using tuple_type = Pass01;
static constexpr auto tupleSize = std::tuple_size_v<tuple_type>;

template <size_t currentIndex = 0> bool getValue(tuple_type& out_value, simdjson::dom::element json_value) {
	if constexpr (currentIndex < tupleSize) {
		if (!getValue(std::get<currentIndex>(out_value), json_value)) {
			return false;
		}
		return getValue<currentIndex + 1>(out_value, json_value);
	}
	return true;
}

template<unique_ptr_t value_type> bool getValue(value_type& e, simdjson::dom::element jsonData) {
	if (!e) {
		e = std::make_unique<std::remove_cvref_t<decltype(*e)>>();
	}
	return getValue(*e, jsonData);
}

template<shared_ptr_t value_type> bool getValue(value_type& e, simdjson::dom::element jsonData) {
	if (!e) {
		e = std::make_shared<std::remove_cvref_t<decltype(*e)>>();
	}
	return getValue(*e, jsonData);
}

template<> bool getValue(status_data& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	if (!getValue(value.metadata, obj, "metadata")) {
		return false;
	}
	if (!getValue(value.created_at, obj, "created_at")) {
		return false;
	}
	if (!getValue(value.id, obj, "id")) {
		return false;
	}
	if (!getValue(value.id_str, obj, "id_str")) {
		return false;
	}
	if (!getValue(value.text, obj, "text")) {
		return false;
	}
	if (!getValue(value.source, obj, "source")) {
		return false;
	}
	if (!getValue(value.truncated, obj, "truncated")) {
		return false;
	}
	if (!getValue(value.in_reply_to_status_id, obj, "in_reply_to_status_id")) {
		return false;
	};
	if (!getValue(value.in_reply_to_status_id_str, obj, "in_reply_to_status_id_str")) {
		return false;
	};
	if (!getValue(value.in_reply_to_user_id, obj, "in_reply_to_user_id")) {
		return false;
	};
	if (!getValue(value.in_reply_to_user_id_str, obj, "in_reply_to_user_id_str")) {
		return false;
	};
	if (!getValue(value.in_reply_to_screen_name, obj, "in_reply_to_screen_name")) {
		return false;
	};
	if (!getValue(value.user, obj, "user")) {
		return false;
	};
	if (!getValue(value.geo, obj, "geo")) {
		return false;
	};
	if (!getValue(value.coordinates, obj, "coordinates")) {
		return false;
	};
	if (!getValue(value.place, obj, "place")) {
		return false;
	};
	if (!getValue(value.contributors, obj, "contributors")) {
		return false;
	};
	if (!getValue(value.retweet_count, obj, "retweet_count")) {
		return false;
	};
	if (!getValue(value.favorite_count, obj, "favorite_count")) {
		return false;
	};
	if (!getValue(value.entities, obj, "entities")) {
		return false;
	};
	if (!getValue(value.favorited, obj, "favorited")) {
		return false;
	};
	if (!getValue(value.retweeted, obj, "retweeted")) {
		return false;
	};
	if (!getValue(value.lang, obj, "lang")) {
		return false;
	};
	if (!getValue(value.retweeted_status, obj, "retweeted_status")) {
		return false;
	};
	if (!getValue(value.possibly_sensitive, obj, "possibly_sensitive")) {
		return false;
	};

	return true;
}

template<> bool getValue(twitter_message& value, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	return getValue(value.statuses, obj, "statuses") && getValue(value.search_metadata, obj, "search_metadata");
}

template<> bool getValue(audience_sub_category_names& p, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();
	return getValue(p.the337100890, obj, "337100890");
}

template<> bool getValue(names&, simdjson::dom::element) {
	return true;
}

template<> bool getValue(event& e, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(e.description, obj, "description") && getValue(e.id, obj, "id") && getValue(e.logo, obj, "logo") && getValue(e.name, obj, "name") &&
		getValue(e.topicIds, obj, "subTopicIds") && getValue(e.subjectCode, obj, "subjectCode") && getValue(e.subtitle, obj, "subtitle") &&
		getValue(e.topicIds, obj, "topicIds");
}

template<> bool getValue(price& p, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(p.amount, obj, "amount") && getValue(p.audienceSubCategoryId, obj, "audienceSubCategoryId") && getValue(p.seatCategoryId, obj, "seatCategoryId");
}

template<> bool getValue(area& a, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(a.areaId, obj, "areaId") && getValue(a.blockIds, obj, "blockIds");
}

template<> bool getValue(seat_category& sc, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(sc.areas, obj, "areas") && getValue(sc.seatCategoryId, obj, "seatCategoryId");
}

template<> bool getValue(venue_names& vn, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(vn.PLEYEL_PLEYEL, obj, "PLEYEL_PLEYEL");
}

template<> bool getValue(performance& p, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(p.eventId, obj, "eventId") && getValue(p.eventId, obj, "id") && getValue(p.logo, obj, "logo") && getValue(p.name, obj, "name") &&
		getValue(p.prices, obj, "prices") && getValue(p.seatCategories, obj, "seatCategories") && getValue(p.seatMapImage, obj, "seatMapImage") &&
		getValue(p.start, obj, "start") && getValue(p.venueCode, obj, "venueCode");
}

template<> bool getValue(citm_catalog_message& msg, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	if (!getValue(msg.areaNames, obj, "areaNames"))
		return false;
	if (!getValue(msg.audienceSubCategoryNames, obj, "audienceSubCategoryNames"))
		return false;
	if (!getValue(msg.blockNames, obj, "blockNames"))
		return false;
	if (!getValue(msg.events, obj, "events"))
		return false;
	if (!getValue(msg.performances, obj, "performances"))
		return false;
	if (!getValue(msg.seatCategoryNames, obj, "seatCategoryNames"))
		return false;
	if (!getValue(msg.subTopicNames, obj, "subTopicNames"))
		return false;
	if (!getValue(msg.subjectNames, obj, "subjectNames"))
		return false;
	if (!getValue(msg.topicNames, obj, "topicNames"))
		return false;
	if (!getValue(msg.topicSubTopics, obj, "topicSubTopics"))
		return false;
	if (!getValue(msg.venueNames, obj, "venueNames"))
		return false;

	return true;
}

template<> bool getValue(geometry_data& geometry, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(geometry.type, obj, "type") && getValue(geometry.coordinates, obj, "coordinates");
}

template<> bool getValue(properties_data& properties, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(properties.name, obj, "name");
}

template<> bool getValue(feature& f, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(f.type, obj, "type") && getValue(f.properties, obj, "properties") && getValue(f.geometry, obj, "geometry");
}

template<> bool getValue(canada_message& message, simdjson::dom::element jsonData) {
	auto newObj = jsonData.get_object();
	if (newObj.error()) {
		return false;
	}
	auto obj = newObj.value();

	return getValue(message.type, obj, "type") && getValue(message.features, obj, "features");
}

template<> bool getValue(Empty&, simdjson::dom::element) {
	return true;
}

template<> bool getValue(Special&, simdjson::dom::element) {
	return true;
}

template<> bool getValue(Obj2& message, simdjson::dom::element json_value) {
	return getValue(message.foo, json_value, "foo");
}

template<> bool getValue(Obj3& message, simdjson::dom::element json_value) {
	return getValue(message.a, json_value, "a") &&
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
					if (getValue(result->data, result->doc)) {
						reply = std::move(result);
						return true;
					}
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