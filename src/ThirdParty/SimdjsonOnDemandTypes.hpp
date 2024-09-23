
#include "test.h"
#include "simdjson.h"
#include <iostream>
#include <sstream>
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

template<typename value_type> void getValue(value_type& data, simdjson::ondemand::value jsonData);

void throwError(auto error, std::source_location location = std::source_location::current()) {
	std::stringstream stream{};
	stream << "Error: " << error << std::endl;
	stream << "Thrown from: " << location.file_name() << ", At Line: " << location.line() << std::endl;
	std::cout << stream.str();
	throw std::runtime_error{ stream.str() };
}

template<floating_type value_type> void getValue(value_type& data, simdjson::ondemand::value jsonData) {
	double newValue{};
	if (auto result = jsonData.get(newValue); result) {
		throwError(result);
	}
	data = static_cast<value_type>(newValue);
}

template<unsigned_type value_type>
void getValue(value_type& data, simdjson::ondemand::value jsonData) {
	uint64_t newValue{};
	if (auto result = jsonData.get(newValue); result) {
		throwError(result);
	}
	data = static_cast<value_type>(newValue);
}

template<signed_type value_type> void getValue(value_type& data, simdjson::ondemand::value jsonData) {
	int64_t newValue{};
	if (auto result = jsonData.get(newValue); result) {
		throwError(result);
	}
	data = static_cast<value_type>(newValue);
}

template<> void getValue(std::nullptr_t&, simdjson::ondemand::value value) {
	if (auto result = value.is_null(); !result.value()) {
		throwError(result);
	}
}

template<> void getValue(std::string& data, simdjson::ondemand::value jsonData) {
	std::string_view newValue;
	if (auto result = jsonData.get(newValue); result) {
		throwError(result);
	}
	data = static_cast<std::string>(newValue);
}

template<bool_t value_type> void getValue(value_type& data, simdjson::ondemand::value jsonData) {
	bool newValue;
	if (auto result = jsonData.get(newValue); result) {
		throwError(result);
	}
	data = static_cast<value_type>(newValue);
}

template<vector_t value_type> void getValue(value_type& value, simdjson::ondemand::value jsonData) {
	const auto size = value.size();
	auto newArray = jsonData.get_array();
	auto iter = newArray.begin();
	typename value_type::value_type valueNew;
	for (size_t x = 0; (x < size) && (iter != newArray.end()); ++x, ++iter) {
		getValue(valueNew, *iter);
		value[x] = std::move(valueNew);
	}
	for (; iter != newArray.end(); ++iter) {
		getValue(valueNew, *iter);
		value.emplace_back(std::move(valueNew));
	}

}

template<typename value_type> void getValue(std::optional<value_type>& vec, simdjson::ondemand::value jsonData) {
	if (!jsonData.is_null()) {
		getValue(vec.emplace(), jsonData);
	}

}

template<map_t map_type> void getValue(map_type& map, simdjson::ondemand::value json_value) {
	simdjson::ondemand::object object;
	if (auto result = json_value.get_object().get(object); result) {
		throwError(result);
	}
	for (auto field : object) {
		typename map_type::key_type key;
		auto key_result = field.unescaped_key();
		if (key_result.error()) {
			return;
		}

		key = static_cast<typename map_type::key_type>(std::string(key_result.value()));

		simdjson::ondemand::value field_value = field.value();
		typename map_type::mapped_type newValue;
		getValue(newValue, field_value);
		map[key] = newValue;
	}


}

template<typename value_type> void getValue(value_type& value, simdjson::ondemand::object jsonData, const std::string_view& key) {
	simdjson::ondemand::value jsonValue;
	if (auto result = jsonData[key].get(jsonValue); !result) {
		return getValue(value, jsonValue);
	}
	else {
		throwError(result);
	}
}

template<> void getValue(hashtag_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
	getValue(value.indices, obj, "indices");
	getValue(value.text, obj, "text");
}

template<> void getValue(large_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
	getValue(value.w, obj, "w");
	getValue(value.h, obj, "h");
	getValue(value.resize, obj, "resize");
}

template<> void getValue(sizes_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
	getValue(value.medium, obj, "medium");
	getValue(value.small, obj, "small");
	getValue(value.thumb, obj, "thumb");
	getValue(value.large, obj, "large");
}

template<> void getValue(media_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

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

template<> void getValue(url_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(value.url, obj, "url");
	getValue(value.expanded_url, obj, "expanded_url");
	getValue(value.display_url, obj, "display_url");
	getValue(value.indices, obj, "indices");
}

template<> void getValue(user_mention& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(value.screen_name, obj, "screen_name");
	getValue(value.name, obj, "name");
	getValue(value.id, obj, "id");
	getValue(value.id_str, obj, "id_str");
	getValue(value.indices, obj, "indices");
}

template<> void getValue(status_entities& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
	getValue(value.media, obj, "media");
	getValue(value.hashtags, obj, "hashtags");
	getValue(value.symbols, obj, "symbols");
	getValue(value.urls, obj, "urls");
	getValue(value.user_mentions, obj, "user_mentions");
}

template<> void getValue(metadata_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(value.result_type, obj, "result_type");
	getValue(value.iso_language_code, obj, "iso_language_code");
}

template<> void getValue(search_metadata_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

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

template<> void getValue(description_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
	return getValue(value.urls, obj, "urls");
}

template<> void getValue(user_entities& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
	getValue(value.url, obj, "url");
	return getValue(value.description, obj, "description");
}

template<> void getValue(twitter_user_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
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

template<shared_ptr_t value_type> void getValue(value_type& e, simdjson::ondemand::value jsonData) {
	if (!e) {
		e = std::make_shared<std::remove_cvref_t<decltype(*e)>>();
	}
	return getValue(*e, jsonData);
}

template<> void getValue(status_data& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(value.metadata, obj, "metadata");
	getValue(value.created_at, obj, "created_at");
	getValue(value.id, obj, "id");
	getValue(value.id_str, obj, "id_str");
	getValue(value.text, obj, "text");
	getValue(value.source, obj, "source");
	getValue(value.truncated, obj, "truncated");
	getValue(value.in_reply_to_status_id, obj, "in_reply_to_status_id");;
	getValue(value.in_reply_to_status_id_str, obj, "in_reply_to_status_id_str");;
	getValue(value.in_reply_to_user_id, obj, "in_reply_to_user_id");;
	getValue(value.in_reply_to_user_id_str, obj, "in_reply_to_user_id_str");;
	getValue(value.in_reply_to_screen_name, obj, "in_reply_to_screen_name");;
	getValue(value.user, obj, "user");;
	getValue(value.geo, obj, "geo");;
	getValue(value.coordinates, obj, "coordinates");;
	getValue(value.place, obj, "place");;
	getValue(value.contributors, obj, "contributors");;
	getValue(value.retweet_count, obj, "retweet_count");;
	getValue(value.favorite_count, obj, "favorite_count");;
	getValue(value.entities, obj, "entities");;
	getValue(value.favorited, obj, "favorited");;
	getValue(value.retweeted, obj, "retweeted");;
	getValue(value.lang, obj, "lang");;
	getValue(value.retweeted_status, obj, "retweeted_status");;
	getValue(value.possibly_sensitive, obj, "possibly_sensitive");
}

template<> void getValue(twitter_message& value, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
	getValue(value.statuses, obj, "statuses");
	getValue(value.search_metadata, obj, "search_metadata");
}

template<> void getValue(audience_sub_category_names& p, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();
	return getValue(p.the337100890, obj, "337100890");
}

template<> void getValue(names&, simdjson::ondemand::value) {

}

template<> void getValue(event& e, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(e.description, obj, "description");
	getValue(e.id, obj, "id");
	getValue(e.logo, obj, "logo");
	getValue(e.name, obj, "name");
	getValue(e.topicIds, obj, "subTopicIds");
	getValue(e.subjectCode, obj, "subjectCode");
	getValue(e.subtitle, obj, "subtitle");
	getValue(e.topicIds, obj, "topicIds");
}

template<> void getValue(price& p, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(p.amount, obj, "amount");
	getValue(p.audienceSubCategoryId, obj, "audienceSubCategoryId");
	getValue(p.seatCategoryId, obj, "seatCategoryId");
}

template<> void getValue(area& a, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(a.areaId, obj, "areaId");
	getValue(a.blockIds, obj, "blockIds");
}

template<> void getValue(seat_category& sc, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(sc.areas, obj, "areas");
	getValue(sc.seatCategoryId, obj, "seatCategoryId");
}

template<> void getValue(venue_names& vn, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	return getValue(vn.PLEYEL_PLEYEL, obj, "PLEYEL_PLEYEL");
}

template<> void getValue(performance& p, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

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

template<> void getValue(citm_catalog_message& msg, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

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

template<> void getValue(geometry_data& geometry, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(geometry.type, obj, "type");
	getValue(geometry.coordinates, obj, "coordinates");
}

template<> void getValue(properties_data& properties, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	return getValue(properties.name, obj, "name");
}

template<> void getValue(feature& f, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(f.type, obj, "type");
	getValue(f.properties, obj, "properties");
	getValue(f.geometry, obj, "geometry");
}

template<> void getValue(canada_message& message, simdjson::ondemand::value jsonData) {
	auto newObj = jsonData.get_object();
	if (auto result = newObj.error(); result) {
		throwError(result);
	}
	auto obj = newObj.value();

	getValue(message.type, obj, "type");
	getValue(message.features, obj, "features");
}

using tuple_type = Pass01;
static constexpr auto tupleSize = std::tuple_size_v<tuple_type>;

template <size_t currentIndex = 0> void getValue(tuple_type& out_value, simdjson::ondemand::value json_value) {
	if constexpr (currentIndex < tupleSize) {
		getValue(std::get<currentIndex>(out_value);
		return getValue<currentIndex + 1>(out_value, json_value);
	}
}

template<unique_ptr_t value_type> void getValue(value_type& e, simdjson::ondemand::value jsonData) {
	if (!e) {
		e = std::make_unique<std::remove_cvref_t<decltype(*e)>>();
	}
	return getValue(*e, jsonData);
}

template<> void getValue(int*& out_value, simdjson::ondemand::value jsonData) {
	int64_t temp{};
	if (auto result = jsonData.get(temp); result) {
		throwError(result);
	}
	out_value = new int(static_cast<int>(temp));

}

template<> void getValue(std::unique_ptr<int>& out_value, simdjson::ondemand::value jsonData) {
	int64_t temp;
	if (auto result = jsonData.get(temp); result) {
		throwError(result);
	}
	out_value = std::make_unique<int>(temp);

}

template<> void getValue(std::string*& value, simdjson::ondemand::value jsonData) {
	if (!value) {
		value = new std::string{};
	}
	return getValue(*value, jsonData);
}

template<> void getValue(Obj2& message, simdjson::ondemand::value json_value) {
	return getValue(message.foo, json_value, "foo");
}

template<> void getValue(Obj3& message, simdjson::ondemand::value json_value) {
	getValue(message.a, json_value, "a");
	getValue(message.foo, json_value, "foo");
}

template<> void getValue(Empty&, simdjson::ondemand::value) {

}

template<> void getValue(Special&, simdjson::ondemand::value) {

}

namespace SimdjsonOnDemandTypes
{
	simdjson::ondemand::parser parser{};
	class VectorDouble : public TestAction
	{
	public:
		virtual bool ParseDouble(const char* json, long double& reply) const
		{
			simdjson::ondemand::document      doc;
			simdjson::padded_string jsonStr(json, strlen(json));
			if (parser.iterate(jsonStr).get(doc) != simdjson::SUCCESS) {
				return false;
			}
			std::vector<double> data;
			getValue(data, doc);
			if (data.size() == 1) {
				reply = data[0];
				return true;
			}
			else {
				return false;
			}
		}
	};

	class VectorString : public TestAction
	{
	public:
		virtual bool ParseString(const char* json, std::string& reply) const
		{
			simdjson::ondemand::document      doc;
			simdjson::padded_string jsonStr(json, strlen(json));
			if (parser.iterate(jsonStr).get(doc) != simdjson::SUCCESS) {
				return false;
			}
			std::vector<std::string> data;
			getValue(data, doc);
			if (data.size() == 1) {
				reply = data[0];
				return true;
			}
			else {
				return false;
			}
		};
	};
};

namespace SimdjsonOnDemandTypes {

	using namespace simdjson;

	template<typename value_type>
	struct SimdOndemandResult : public ParseResultBase
	{
		simdjson::padded_string     json;
		ondemand::document          doc;
		SimdOndemandResult(char const* j, size_t length)
			: json(j, length)
		{}
	};

	template<typename value_type>
	class GetValue : public TestAction
	{
	public:
		GetValue()
		{}

		void streamDoc(ondemand::value element, std::ostream& s) const
		{
			switch (element.type())
			{
			case ondemand::json_type::array:
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
			case ondemand::json_type::object:
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
			case ondemand::json_type::number:
			{
				double value = element.get_double();
				s << std::setprecision(17) << value;
				break;
			}
			case ondemand::json_type::string:
			{
				std::string_view value = element.get_string();
				s << '"' << value << '"';
				break;
			}
			case ondemand::json_type::boolean:
			{
				bool    value = element.get_bool();
				s << (value ? "true" : "false");
				break;
			}
			case ondemand::json_type::null:
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
			std::unique_ptr<SimdOndemandResult<value_type>> result = std::make_unique<SimdOndemandResult<value_type>>(json, length);
			auto error = parser.iterate(result->json).get(result->doc);
			value_type value;
			if (!error) {
				simdjson::ondemand::value valueDoc{ result->doc };
				getValue(value, valueDoc);
				reply = std::move(result);
			}

		}

		virtual bool ParseDouble(const char* json, long double& d) const override
		{
			ondemand::document      doc;
			simdjson::padded_string jsonStr(json, strlen(json));
			if (parser.iterate(jsonStr).get(doc) != SUCCESS) {

			}
			auto array = doc.get_array();
			for (auto val : array) {
				d = val.get_double();
			}

		}

		virtual bool ParseString(const char* json, std::string& s) const override
		{
			ondemand::document      doc;
			simdjson::padded_string jsonStr(json, strlen(json));
			if (parser.iterate(jsonStr).get(doc) != SUCCESS) {

			}
			auto array = doc.get_array();
			for (auto val : array) {
				std::string_view view = val.get_string();
				s = std::string(std::begin(view), std::end(view));
			}

		}

		virtual bool Stringify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
		{
			return Prettify(parseResult, reply);
		}
		virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
		{
			SimdOndemandResult<value_type> const& input = dynamic_cast<SimdOndemandResult<value_type> const&>(parseResult);
			ondemand::document          doc;
			auto error = parser.iterate(input.json).get(doc);
			if (!error) {
				ondemand::value const val = doc;
				std::unique_ptr<StringResultUsingStream> result = std::make_unique<StringResultUsingStream>();
				streamDoc(val, result->stream);
				reply = std::move(result);
			}

		}
		// virtual bool SaxRoundtrip(const char* json, size_t length, std::unique_ptr<StringResultBase>& reply) const override
		// virtual bool Prettify(const ParseResultBase& parseResult, std::unique_ptr<StringResultBase>& reply) const override
		// virtual bool Statistics(const ParseResultBase& parseResult, Stat& stat) const override
		// virtual bool SaxStatistics(const char* json, size_t length, Stat& stat) const override
		// virtual bool SaxStatisticsUTF16(const char* json, size_t length, Stat& stat) const override
	};

}
