// Copyright (C) 2011-2012 by the Bem++ Authors
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

#ifndef fiber_default_local_assembler_for_identity_operator_on_surface_hpp
#define fiber_default_local_assembler_for_identity_operator_on_surface_hpp

#include "../common/common.hpp"

#include "local_assembler_for_operators.hpp"
#include "numerical_quadrature.hpp"
#include "numerical_test_trial_integrator.hpp"
#include "shared_ptr.hpp"

#include "../common/armadillo_fwd.hpp"
#include "../common/boost_ptr_map_fwd.hpp"
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <utility>
#include <vector>

namespace Fiber
{

/** \cond FORWARD_DECL */
class OpenClHandler;
/** \endcond */

template <typename BasisFunctionType, typename ResultType, typename GeometryFactory>
class DefaultLocalAssemblerForIdentityOperatorOnSurface :
    public LocalAssemblerForOperators<ResultType>
{
public:
    typedef typename ScalarTraits<ResultType>::RealType CoordinateType;

    DefaultLocalAssemblerForIdentityOperatorOnSurface(
        const shared_ptr<const GeometryFactory>& geometryFactory,
        const shared_ptr<const RawGridGeometry<CoordinateType> >& rawGeometry,
        const shared_ptr<const std::vector<const Basis<BasisFunctionType>*> >& testBases,
        const shared_ptr<const std::vector<const Basis<BasisFunctionType>*> >& trialBases,
        const shared_ptr<const CollectionOfBasisTransformations<CoordinateType> >& testTransformations,
        const shared_ptr<const CollectionOfBasisTransformations<CoordinateType> >& trialTransformations,
        const shared_ptr<const OpenClHandler>& openClHandler);

    virtual void evaluateLocalWeakForms(
        CallVariant callVariant,
        const std::vector<int>& elementIndicesA,
        int elementIndexB,
        LocalDofIndex localDofIndexB,
        std::vector<arma::Mat<ResultType> >& result,
        CoordinateType nominalDistance = -1.);

    virtual void evaluateLocalWeakForms(
        const std::vector<int>& testElementIndices,
        const std::vector<int>& trialElementIndices,
        Fiber::_2dArray<arma::Mat<ResultType> >& result,
        CoordinateType nominalDistance = -1.);

    virtual void evaluateLocalWeakForms(
        const std::vector<int>& elementIndices,
        std::vector<arma::Mat<ResultType> >& result);

private:
    void checkConsistencyOfGeometryAndBases(
            const RawGridGeometry<CoordinateType>& rawGeometry,
            const std::vector<const Basis<BasisFunctionType>*>& bases) const;

    const TestTrialIntegrator<BasisFunctionType, ResultType>&
    selectIntegrator(int elementIndex);

    const TestTrialIntegrator<BasisFunctionType, ResultType>& getIntegrator(
        const SingleQuadratureDescriptor& desc);
private:
    typedef boost::ptr_map<SingleQuadratureDescriptor,
            TestTrialIntegrator<BasisFunctionType, ResultType> > IntegratorMap;

private:
    shared_ptr<const GeometryFactory> m_geometryFactory;
    shared_ptr<const RawGridGeometry<CoordinateType> > m_rawGeometry;
    shared_ptr<const std::vector<const Basis<BasisFunctionType>*> > m_testBases;
    shared_ptr<const std::vector<const Basis<BasisFunctionType>*> > m_trialBases;
    shared_ptr<const CollectionOfBasisTransformations<CoordinateType> > m_testTransformations;
    shared_ptr<const CollectionOfBasisTransformations<CoordinateType> > m_trialTransformations;
    shared_ptr<const OpenClHandler> m_openClHandler;

    IntegratorMap m_testTrialIntegrators;
};

} // namespace Fiber

#include "default_local_assembler_for_identity_operator_on_surface_imp.hpp"

#endif
