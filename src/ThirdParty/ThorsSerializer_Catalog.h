#ifndef THORS_SERIALIZER_CATALOG_H
#define THORS_SERIALIZER_CATALOG_H

#include "test.h"
#include "TypeSafeCatalog.h"
#include "ThorSerialize/Traits.h"
#include "ThorSerialize/SerUtil.h"


ThorsAnvil_MakeTrait(Event,         description, id, logo, name, subTopicIds, subjectCode, subtitle, topicIds);
ThorsAnvil_MakeTrait(Price,         amount, audienceSubCategoryId, seatCategoryId);
ThorsAnvil_MakeTrait(Area,          areaId, blockIds);
ThorsAnvil_MakeTrait(SeatCategorie, areas, seatCategoryId);
ThorsAnvil_MakeTrait(Performance,   eventId, id, logo, name, prices, seatCategories, seatMapImage, start, venueCode);
ThorsAnvil_MakeTrait(VenueNames,    PLEYEL_PLEYEL);
ThorsAnvil_MakeTrait(Perform,       areaNames, audienceSubCategoryNames, blockNames, events, performances, seatCategoryNames,
                                    subTopicNames, subjectNames, topicNames, topicSubTopics, venueNames);

#endif
