// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides the configuration for maximum number of errors for all error types.
 * \author Christopher Pockrandt <christopher.pockrandt AT fu-berlin.de>
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 * \author Lydia Buntrock <lydia.buntrock AT fu-berlin.de>
 */

#pragma once

#include <variant>

#include <seqan3/core/algorithm/pipeable_config_element.hpp>
#include <seqan3/search/configuration/detail.hpp>
#include <seqan3/search/configuration/max_error_common.hpp>
#include <seqan3/search/detail/search_common.hpp>

namespace seqan3::search_cfg
{
/*!\brief Configuration element that represents the number or rate of total errors.
 * \ingroup search_configuration
 *
 * \details
 * This configuration element can be used to determine the number or rate of total errors that are supported.
 *
 * ### Example
 * \include test/snippet/search/configuration_error.cpp
 */
class max_error_total : public pipeable_config_element<max_error_total, std::variant<error_count, error_rate>>
{
public:
    //!\privatesection
    //!\brief Internal id to check for consistent configuration settings.
    static constexpr detail::search_config_id id{detail::search_config_id::max_error_total};
};

/*!\brief Configuration element that represents the number or rate of substitution errors.
 * \ingroup search_configuration
 *
 * \details
 * This configuration element can be used to determine the number or rate of substitution errors that are supported.
 * A substitution corresponds to diverging bases between text and query for a certain position.
 *
 * ### Example
 * \include test/snippet/search/configuration_error.cpp
 */
class max_error_substitution : public pipeable_config_element<max_error_substitution,
                                                              std::variant<error_count,
                                                              error_rate>>
{
public:
    //!\privatesection
    //!\brief Internal id to check for consistent configuration settings.
    static constexpr detail::search_config_id id{detail::search_config_id::max_error_substitution};
};

/*!\brief Configuration element that represents the number or rate of insertion errors.
 * \ingroup search_configuration
 *
 * \details
 * This configuration element can be used to determine the number or rate of insertion errors that are supported.
 * An insertion corresponds to a base inserted into the query that does not occur in the text at the position.
 *
 * ### Example
 * \include test/snippet/search/configuration_error.cpp
 */
class max_error_insertion : public pipeable_config_element<max_error_insertion, std::variant<error_count, error_rate>>
{
public:
    //!\privatesection
    //!\brief Internal id to check for consistent configuration settings.
    static constexpr detail::search_config_id id{detail::search_config_id::max_error_insertion};
};

/*!\brief Configuration element that represents the number or rate of deletion errors.
 * \ingroup search_configuration
 *
 * \details
 * This configuration element can be used to determine the number or rate of deletion errors that are supported.
 * A deletion corresponds to a base deleted from the query sequence that does occur in the text.
 * Deletions at the beginning and at the end of the sequence are not considered during a search.
 *
 * ### Example
 * \include test/snippet/search/configuration_error.cpp
 */
class max_error_deletion : public pipeable_config_element<max_error_deletion, std::variant<error_count, error_rate>>
{
public:
    //!\privatesection
    //!\brief Internal id to check for consistent configuration settings.
    static constexpr detail::search_config_id id{detail::search_config_id::max_error_deletion};
};

} // namespace seqan3::search_cfg
