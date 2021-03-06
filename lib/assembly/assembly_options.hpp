// Copyright (C) 2011-2012 by the BEM++ Authors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef bempp_assembly_options_hpp
#define bempp_assembly_options_hpp

#include "../common/common.hpp"

#include <string>

#include "../common/deprecated.hpp"

#include "../fiber/opencl_options.hpp"
#include "../fiber/parallelization_options.hpp"
#include "../fiber/verbosity_level.hpp"

namespace Bempp
{

/** \ingroup weak_form_assembly
 *  \brief Adaptive cross approximation (ACA) parameters.
 */
struct AcaOptions
{
    /** \brief Initialize ACA parameters to default values. */
    AcaOptions();

    /** \brief Estimate of the desired approximation accuracy.
     *
     *  Default value: 1e-4. */
    double eps;
    /** \brief Cluster-pair admissibility parameter.
     *
     *  Default value: 1.2. */
    double eta;
    /** \brief Minimum size of blocks approximated with ACA.
     *
     *  Matrix blocks whose smaller side is less than this value will be stored
     *  in the dense format (ACA will not be attempted on these blocks).
     *
     *  Default value: 16. */
    unsigned int minimumBlockSize;
    /** \brief Maximum allowed block size.
     *
     *  Matrix blocks with side larger than this value will be split. This can
     *  be used to limit the time devoted to (serial) processing of an
     *  individual block, especially when a large number of threads is used.
     *
     *  Default value: UINT_MAX. */
    unsigned int maximumBlockSize;
    /** \brief Maximum rank of blocks stored in the low-rank format.
     *
     *  Blocks judged to have higher rank will be stored in the dense format.
     *
     *  Default value: UINT_MAX. */
    unsigned int maximumRank;
    /** \brief Do global assembly before ACA?
     *
     *  If true, ACA is performed on the assembled operator (i.e. the matrix is
     *  indexed with global degrees of freedom). Otherwise ACA is performed on
     *  matrix indexed with local degrees of freedom, and the global assembly
     *  is done by pre- and postmultiplying the H matrix with sparse matrices
     *  mapping local DOFs to global DOFs and vice versa.
     *
     *  Default value: true. */
    bool globalAssemblyBeforeCompression;
    /** \brief Recompress ACA matrix after construction?
     *
     *  If true, blocks of H matrices are agglomerated in an attempt to reduce
     *  memory consumption.
     *
     *  Warning: this procedure is not parallelised yet, therefore it may be
     *  slow.
     *
     *  Default value: false. */
    bool recompress;
    /** \brief If true, hierarchical matrix structure will be written in
     *  PostScript format at the end of the assembly procedure.
     *
     *  Default value: false. */
    bool outputPostscript;
    /** \brief Name of the output PostScript file.
     *
     *  \see outputPostscript.
     *
     *  Default value: "aca.ps". */
    std::string outputFname;
    /** \brief Estimate of the magnitude of typical entries of the matrix to be
     *  approximated.
     *
     *  Usually does not need to be changed. Default value: 1. */
    double scaling;
};

using Fiber::OpenClOptions;
using Fiber::ParallelizationOptions;
using Fiber::VerbosityLevel;

/** \ingroup weak_form_assembly
 *  \brief Options determining how weak-form assembly is done.
 */
class AssemblyOptions
{
public:
    enum { AUTO = -1 };

    AssemblyOptions();

    /** @name Assembly mode
      @{ */

    /** \brief Possible assembly modes for weak forms of boundary integral operators. */
    enum Mode {
        /** \brief Assemble dense matrices. */
        DENSE,
        /** \brief Assemble hierarchical matrices using adaptive cross approximation (ACA). */
        ACA
    };

    /** \brief Use dense-matrix representations of weak forms of boundary integral operators.
     *
     *  This is the default assembly mode. */
    void switchToDenseMode();

    /** \brief Use adaptive cross approximation (ACA) to obtain hierarchical-matrix
     *  representations of weak forms of boundary integral operators.
     *
     *  \param[in] acaOptions Parameters influencing the ACA algorithm. */
    void switchToAcaMode(const AcaOptions& acaOptions);

    /** \brief Use dense-matrix representations of weak forms of boundary integral operators.
     *
     *  \deprecated Use switchToDenseMode() instead. */
    BEMPP_DEPRECATED void switchToDense();

    /** \brief Use adaptive cross approximation (ACA) to obtain hierarchical-matrix
     *  representations of weak forms of boundary integral operators.
     *
     *  \deprecated Use switchToAcaMode() instead. */
    BEMPP_DEPRECATED void switchToAca(const AcaOptions& acaOptions);

    /** \brief Current assembly mode.
     *
     *  The assembly mode can be changed by calling switchToDenseMode() or
     *  switchToAcaMode(). */
    Mode assemblyMode() const;

    /** \brief Return the current adaptive cross approximation (ACA) settings.
     *
     *  \note These settings are only used in the ACA assembly mode, i.e. when
     *  assemblyMode() returns ACA. */
    const AcaOptions& acaOptions() const;

    /** @}
      @name Parallelization
      @{ */

    // Temporarily removed (OpenCl support is broken).
    // void enableOpenCl(const OpenClOptions& openClOptions);
    // void disableOpenCl();

    /** \brief Set the maximum number of threads used during the assembly.
     *
     *  \p maxThreadCount must be a positive number or \p AUTO. In the latter
     *  case the number of threads is determined automatically. */
    void setMaxThreadCount(int maxThreadCount);

    /** \brief Set the maximum number of threads used during the assembly.
     *
     *  \deprecated Use setMaxThreadCount() instead. */
    BEMPP_DEPRECATED void switchToTbb(int maxThreadCount = AUTO);

    /** \brief Return current parallelization options. */
    const ParallelizationOptions& parallelizationOptions() const;

    /** @}
      @name Verbosity
      */

    /** \brief Set the verbosity level.
     *
     *  This setting determines the amount of information printed out by
     *  functions from BEM++. */
    void setVerbosityLevel(VerbosityLevel::Level level);

    /** \brief Return the verbosity level. */
    VerbosityLevel::Level verbosityLevel() const;

    /** @}
      @name Miscellaneous
      @{ */

    /** \brief Specify whether singular integrals are cached during weak-form assembly.
     *
     *  If <tt>value == true</tt>, singular integrals are precalculated
     *  and stored in a cache before filling the matrix of the discretized weak
     *  form of a singular boundary operator. Otherwise these integrals
     *  are evaluated as needed during the assembly of the weak form.
     *
     *  By default, singular integral caching is enabled. */
    void enableSingularIntegralCaching(bool value = true);

    /** \brief Return whether singular integrals should be cached during weak-form assembly.
     *
     *  See enableSingularIntegralCaching() for more information. */
    bool isSingularIntegralCachingEnabled() const;

    /** \brief Specify whether mass matrices should be stored in sparse format.
     *
     *  If <tt>value == true</tt>, assembled mass matrices are stored as sparse
     *  matrices. Otherwise they are stored as dense matrices.
     *
     *  By default, sparse storage of mass matrices is enabled. */
    void enableSparseStorageOfMassMatrices(bool value = true);

    /** \brief Return whether mass matrices should be stored in sparse format.
     *
     *  See enableSparseStorageOfMassMatrices() for more information. */
    bool isSparseStorageOfMassMatricesEnabled() const;

    /** \brief Enable or disable joint assembly of integral-operator superpositions.
     *
     *  If <tt>value == true</tt>, the discrete weak forms of superpositions of
     *  elementary integral operators, such as 5. * SLP + 2. * DLP, will
     *  assembled together (for example, in a single run of ACA) and stored as a
     *  single H-matrix or dense matrix. By default joint assembly is disabled,
     *  which means that the discrete weak form of each elementary operator is
     *  stored separately. */
    void enableJointAssembly(bool value = true);

    /** \brief Return whether joint assembly of integral-operator superpositions
     *  is enabled
     *
     * See enableJointAssembly() for more information. */
    bool isJointAssemblyEnabled() const;

    /** @} */

private:
    /** \cond */
    Mode m_assemblyMode;
    AcaOptions m_acaOptions;
    ParallelizationOptions m_parallelizationOptions;
    VerbosityLevel::Level m_verbosityLevel;
    bool m_singularIntegralCaching;
    bool m_sparseStorageOfMassMatrices;
    bool m_jointAssembly;
    /** \endcond */
};

} // namespace Bempp

#endif
