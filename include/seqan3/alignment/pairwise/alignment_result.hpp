// -----------------------------------------------------------------------------------------------------
// Copyright (c) 2006-2020, Knut Reinert & Freie Universität Berlin
// Copyright (c) 2016-2020, Knut Reinert & MPI für molekulare Genetik
// This file may be used, modified and/or redistributed under the terms of the 3-clause BSD-License
// shipped with this file and also available at: https://github.com/seqan/seqan3/blob/master/LICENSE.md
// -----------------------------------------------------------------------------------------------------

/*!\file
 * \brief Provides seqan3::alignment_result.
 * \author Jörg Winkler <j.winkler AT fu-berlin.de>
 * \author Rene Rahn <rene.rahn AT fu-berlin.de>
 */

#pragma once

#include <optional>

#include <seqan3/core/algorithm/configuration.hpp>
#include <seqan3/core/detail/debug_stream_type.hpp>
#include <seqan3/core/type_traits/template_inspection.hpp>

namespace seqan3::detail
{

// forward declaration for friend declaraion in alignment_result.
template <typename>
#if !SEQAN3_WORKAROUND_GCC_93467
//!\cond
    requires is_type_specialisation_of_v<configuration_t, configuration>
//!\endcond
#endif // !SEQAN3_WORKAROUND_GCC_93467
class policy_alignment_result_builder;

/*!\brief A struct that contains the actual alignment result data.
 * \ingroup pairwise_alignment
 *
 * \tparam id_t                  The type for the alignment identifier.
 * \tparam score_t               The type for the resulting score.
 * \tparam end_positions_t          The type for the end positions, can be omitted.
 * \tparam begin_positions_t         The type for the begin positions, can be omitted.
 * \tparam alignment_t           The type for the alignment, can be omitted.
 * \tparam score_debug_matrix_t  The type for the score matrix. Only present if seqan3::align_cfg::debug is enabled.
 * \tparam trace_debug_matrix_t  The type for the trace matrix. Only present if seqan3::align_cfg::debug is enabled.
 */
template <typename id_t,
          typename score_t,
          typename end_positions_t = std::nullopt_t *,
          typename begin_positions_t = std::nullopt_t *,
          typename alignment_t = std::nullopt_t *,
          typename score_debug_matrix_t = std::nullopt_t *,
          typename trace_debug_matrix_t = std::nullopt_t *>
struct alignment_result_value_type
{
    //! \brief The alignment identifier.
    id_t id{};
    //! \brief The alignment score.
    score_t score{};
    //! \brief The end positions of the alignment.
    end_positions_t end_positions{};
    //! \brief The begin positions of the alignment.
    begin_positions_t begin_positions{};
    //! \brief The alignment, i.e. the actual base pair matching.
    alignment_t alignment{};

    //!\brief The score matrix. Only accessible with seqan3::align_cfg::debug.
    score_debug_matrix_t score_debug_matrix{};
    //!\brief The trace matrix. Only accessible with seqan3::align_cfg::debug.
    trace_debug_matrix_t trace_debug_matrix{};
};

/*!\name Type deduction guides
 * \brief Type deduction for the different combinations of result types.
 * \{
 */
 //! \brief Type deduction for an empty object. It will always fail the compilation, if any field is accessed.
alignment_result_value_type()
    -> alignment_result_value_type<std::nullopt_t *, std::nullopt_t *>;

//! \brief Type deduction for id and score only.
template <typename id_t, typename score_t>
alignment_result_value_type(id_t, score_t)
    -> alignment_result_value_type<id_t, score_t>;

//! \brief Type deduction for id, score and end positions.
template <typename id_t, typename score_t, typename end_positions_t>
alignment_result_value_type(id_t, score_t, end_positions_t)
    -> alignment_result_value_type<id_t, score_t, end_positions_t>;

//! \brief Type deduction for id, score, end positions and begin positions.
template <typename id_t, typename score_t, typename end_positions_t, typename begin_positions_t>
alignment_result_value_type(id_t, score_t, end_positions_t, begin_positions_t)
    -> alignment_result_value_type<id_t, score_t, end_positions_t, begin_positions_t>;

//! \brief Type deduction for id, score, end positions, begin positions and alignment.
template <typename id_t, typename score_t, typename end_positions_t, typename begin_positions_t, typename alignment_t>
alignment_result_value_type(id_t, score_t, end_positions_t, begin_positions_t, alignment_t)
    -> alignment_result_value_type<id_t, score_t, end_positions_t, begin_positions_t, alignment_t>;
//!\}

//!\cond
template <typename result_t>
struct alignment_result_value_type_accessor;
//!\endcond
} // namespace seqan3::detail

namespace seqan3
{

/*!\brief Stores the alignment results and gives access to score, alignment and the front and end positionss.
 * \ingroup pairwise_alignment
 * \tparam alignment_result_value_t The underlying value type containing the information from the alignment computation.
 *
 * \details
 *
 * This class provides read-only access to the results of a pairwise alignment computation.
 * It always contains an alignment identifier and the resulting score.
 * Optionally – if the user requests – also the begin and end positions within
 * the sequences and the alignment can be calculated. When accessing a field that
 * has not been calculated, an assertion will fail during compilation.
 *
 * \remark The template type argument is set internally by a result factory and depends on the configuration of the
 * alignment result. For the general use of this class the concrete type information is not relevant and it
 * suffices to know that this is a template class with one template type argument.
 *
 * \if DEV
 * To access the type of the passed alignment result value use the seqan3::detail::alignment_result_value_type_accessor
 * transformation trait.
 * The seqan3::detail::policy_alignment_result_builder is used to set the respective
 * values. This class is befriended with this class to allow access to the private data member.
 * \endif
 */
template <typename alignment_result_value_t>
//!\cond
    requires detail::is_type_specialisation_of_v<alignment_result_value_t, detail::alignment_result_value_type>
//!\endcond
class alignment_result
{
private:
    //! \brief Object that stores the computed alignment results.
    alignment_result_value_t data{};

    /*!\name Member types
     * \brief Local definition of the types contained in the `data` object.
     * \{
     */
    //! \brief The type for the alignment identifier.
    using id_t = decltype(data.id);
    //! \brief The type for the resulting score.
    using score_t = decltype(data.score);
    //! \brief The type for the end positions.
    using end_positions_t = decltype(data.end_positions);
    //! \brief The type for the begin positions.
    using begin_positions_t = decltype(data.begin_positions);
    //! \brief The type for the alignment.
    using alignment_t = decltype(data.alignment);
    //!\}

    //!\brief Befriend alignment result builder.
    template <typename configuration_t>
    #if !SEQAN3_WORKAROUND_GCC_93467
    //!\cond
        requires is_type_specialisation_of_v<configuration_t, configuration>
    //!\endcond
    #endif // !SEQAN3_WORKAROUND_GCC_93467
    friend class detail::policy_alignment_result_builder;

public:
    /*!\name Constructors, destructor and assignment
     * \{
     */

    //!\privatesection
    /*!\brief Constructs a seqan3::alignment_result from an `value_type` object.
     * \param[in] value The alignment results.
     */
    alignment_result(alignment_result_value_t value) : data(std::move(value))
    {}

    //!\publicsection
    alignment_result() = default;                                     //!< Defaulted.
    alignment_result(alignment_result const &) = default;             //!< Defaulted.
    alignment_result(alignment_result &&) = default;                  //!< Defaulted.
    alignment_result & operator=(alignment_result const &) = default; //!< Defaulted.
    alignment_result & operator=(alignment_result &&) = default;      //!< Defaulted.
    ~alignment_result() = default;                                    //!< Defaulted.
    //!\}

    /*!\name Access functions
     * \brief Functions to access elements of the alignment result type.
     * \{
     */

    /*!\brief Returns the alignment identifier.
     * \return The id field.
     */
    constexpr id_t id() const noexcept
    {
        static_assert(!std::is_same_v<id_t, std::nullopt_t *>,
                      "Identifier is not available but should.");
        return data.id;
    }

    /*!\brief Returns the alignment score.
     * \return The score field.
     */
    constexpr score_t score() const noexcept
    {
        static_assert(!std::is_same_v<score_t, std::nullopt_t *>,
                      "Alignment score is not available but should.");
        return data.score;
    }

    /*!\brief Returns the end position of the first sequence of the alignment.
     * \return The calculated alignment end of sequence 1 (inclusive).
     *
     * \note This function is only available if the end position of the first sequence was requested via the
     * alignment configuration (see seqan3::align_cfg::result).
     */
    constexpr auto sequence1_end_position() const noexcept
    {
        static_assert(!std::is_same_v<end_positions_t, std::nullopt_t *>,
                      "Trying to access the end position of the first sequence, although it was not requested in the"
                      " alignment configuration.");
        return data.end_positions.first;
    }

    /*!\brief Returns the end position of the second sequence of the alignment.
     * \return A pair of positions in the respective sequences, where the calculated alignment ends (inclusive).
     *
     * \note This function is only available if the end position of the second sequence was requested via the
     * alignment configuration (see seqan3::align_cfg::result).
     */
    constexpr auto sequence2_end_position() const noexcept
    {
        static_assert(!std::is_same_v<end_positions_t, std::nullopt_t *>,
                      "Trying to access the end position of the second sequence, although it was not requested in the"
                      " alignment configuration.");
        return data.end_positions.second;
    }

    /*!\brief Returns the begin position of the first sequence of the alignment.
     * \return  A pair of positions in the respective sequences, where the calculated alignment starts.
     *
     * \details
     *
     * Guaranteed to be smaller than or equal to `sequence1_end_position()`.
     *
     * \note This function is only available if the begin position of the first sequence was requested via the
     * alignment configuration (see seqan3::align_cfg::result).
     */
    constexpr auto sequence1_begin_position() const noexcept
    {
        static_assert(!std::is_same_v<begin_positions_t, std::nullopt_t *>,
                      "Trying to access the begin position of the first sequence, although it was not requested in the"
                      " alignment configuration.");
        return data.begin_positions.first;
    }

    /*!\brief Returns the begin position of the second sequence of the alignment.
     * \return  A pair of positions in the respective sequences, where the calculated alignment starts.
     *
     * \details
     *
     * Guaranteed to be smaller than or equal to `sequence2_end_position()`.
     *
     * \note This function is only available if the begin position of the second sequence was requested via the
     * alignment configuration (see seqan3::align_cfg::result).
     */
    constexpr auto sequence2_begin_position() const noexcept
    {
        static_assert(!std::is_same_v<begin_positions_t, std::nullopt_t *>,
                      "Trying to access the begin position of the second sequence, although it was not requested in the"
                      " alignment configuration.");
        return data.begin_positions.second;
    }

    /*!\brief Returns the actual alignment, i.e. the base pair matching.
     * \return At least two aligned sequences, which represent the alignment.
     *
     * \note This function is only available if the alignment was requested via the alignment configuration
     * (see seqan3::align_cfg::result).
     */
    constexpr alignment_t const & alignment() const noexcept
    {
        static_assert(!std::is_same_v<alignment_t, std::nullopt_t *>,
                      "Trying to access the alignment, although it was not requested in the alignment configuration.");
        return data.alignment;
    }
    //!\}

    //!\cond DEV
    /*!\brief Returns the score matrix used to compute the alignment.
     * \return The score matrix.
     *
     * \details
     *
     * This function is only used for debugging such that performance can be affected significantly when enabling
     * seqan3::align_cfg::debug.
     *
     * \note This function is only available if the debug mode was requested via the alignment configuration
     * (see seqan3::align_cfg::debug).
     */
    constexpr auto const & score_matrix() const noexcept
    {
        static_assert(!std::is_same_v<decltype(data.score_debug_matrix), std::nullopt_t *>,
                      "Trying to access the score matrix, although it was not requested in the alignment configuration.");
        return data.score_debug_matrix;
    }

    /*!\brief Returns the trace matrix used to compute the alignment.
     * \return The score matrix.
     *
     * \details
     *
     * This function is only used for debugging such that performance can be affected significantly when enabling
     * seqan3::align_cfg::debug.
     *
     * \note This function is only available if the debug mode and the alignment was requested via the alignment
     * configuration (see seqan3::align_cfg::debug and seqan3::align_cfg::result).
     */
    constexpr auto const & trace_matrix() const noexcept
    {
        static_assert(!std::is_same_v<decltype(data.trace_debug_matrix), std::nullopt_t *>,
                      "Trying to access the trace matrix, although it was not requested in the alignment configuration.");
        return data.trace_debug_matrix;
    }
    //!\endcond
};
} // namespace seqan3

namespace seqan3::detail
{
/*!\brief Transformation trait to access the hidden result value type of the seqan3::alignment_result class.
 * \implements transformation_trait
 * \ingroup pairwise_alignment
 *
 * \tparam result_t The type of the alignment result.
 *
 * \details
 *
 * The template parameter must be a specialisation of seqan3::alignment_result otherwise this class is incomplete.
 */
template <typename result_value_t>
struct alignment_result_value_type_accessor<alignment_result<result_value_t>>
{
    //!\brief The underlying value type used for the given alignment result type.
    using type = result_value_t;
};

} // namespace seqan3::detail

namespace seqan3
{
/*!\brief Streams the seqan3::alignment_result to the seqan3::debug_stream.
 *
 * \tparam char_t The underlying character type of the seqan3::debug_stream_type.
 * \tparam alignment_result_t A type specialisation of seqan3::alignment_result.
 *
 * \param[in,out] stream The output stream.
 * \param[in] result The alignment result to print.
 * \relates seqan3::debug_stream_type
 */
template <typename char_t, typename alignment_result_t>
//!\cond
    requires detail::is_type_specialisation_of_v<remove_cvref_t<alignment_result_t>, alignment_result>
//!\endcond
inline debug_stream_type<char_t> & operator<<(debug_stream_type<char_t> & stream, alignment_result_t && result)
{
    using result_data_t =
        typename detail::alignment_result_value_type_accessor<remove_cvref_t<alignment_result_t>>::type;

    constexpr bool has_id = !std::is_same_v<decltype(std::declval<result_data_t>().id), std::nullopt_t *>;
    constexpr bool has_score = !std::is_same_v<decltype(std::declval<result_data_t>().score), std::nullopt_t *>;
    constexpr bool has_end_positions = !std::is_same_v<decltype(std::declval<result_data_t>().end_positions),
                                                       std::nullopt_t *>;
    constexpr bool has_begin_positions = !std::is_same_v<decltype(std::declval<result_data_t>().begin_positions),
                                                         std::nullopt_t *>;
    constexpr bool has_alignment = !std::is_same_v<decltype(std::declval<result_data_t>().alignment),
                                                   std::nullopt_t *>;

    stream << '{';
    if constexpr (has_id)
        stream << "id: " << result.id();
    if constexpr (has_score)
        stream << ", score: " << result.score();
    if constexpr (has_begin_positions)
        stream << ", begin: (" << result.sequence1_begin_position() << "," << result.sequence2_begin_position() << ")";
    if constexpr (has_end_positions)
        stream << ", end: (" << result.sequence1_end_position() << "," << result.sequence2_end_position() << ")";
    if constexpr (has_alignment)
        stream << "\nalignment:\n" << result.alignment();
    stream << '}';

    return stream;
}
} // namespace seqan3
