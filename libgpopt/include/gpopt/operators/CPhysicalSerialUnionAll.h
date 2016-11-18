//	Greenplum Database
//	Copyright (C) 2016 Pivotal Software, Inc.

#ifndef GPOPT_CPhysicalSerialUnionAll_H
#define GPOPT_CPhysicalSerialUnionAll_H

#include "gpos/base.h"
#include "gpopt/operators/CPhysicalUnionAll.h"

namespace gpopt
{
	// fwd declaration
	class CDistributionSpecHashed;

	//---------------------------------------------------------------------------
	//	@class:
	//		CPhysicalSerialUnionAll
	//
	//	@doc:
	//		Physical union all operator. Executes each child serially.
	//
	//---------------------------------------------------------------------------
	class CPhysicalSerialUnionAll : public CPhysicalUnionAll
	{

		private:

			// private copy ctor
			CPhysicalSerialUnionAll(const CPhysicalSerialUnionAll &);

		public:

			// ctor
			CPhysicalSerialUnionAll
				(
				IMemoryPool *pmp,
				DrgPcr *pdrgpcrOutput,
				DrgDrgPcr *pdrgpdrgpcrInput,
				ULONG ulScanIdPartialIndex
				);

			// dtor
			virtual
			~CPhysicalSerialUnionAll();

			// ident accessors
			virtual
			EOperatorId Eopid() const
			{
				return EopPhysicalSerialUnionAll;
			}

			virtual
			const CHAR *SzId() const
			{
				return "CPhysicalSerialUnionAll";
			}

			// distribution matching type
			virtual
			CEnfdDistribution::EDistributionMatching Edm
				(
				CReqdPropPlan *prppInput,
				ULONG , // ulChildIndex
				DrgPdp *, //pdrgpdpCtxt
				ULONG ulOptReq
				)
			{
				if (0 == ulOptReq  && CDistributionSpec::EdtHashed == prppInput->Ped()->PdsRequired()->Edt())
				{
					// use exact matching if optimizing first request
					return CEnfdDistribution::EdmExact;
				}

				// use relaxed matching if optimizing other requests
				return CEnfdDistribution::EdmSatisfy;
			}


			// compute required distribution of the n-th child
			virtual
			CDistributionSpec *PdsRequired
				(
					IMemoryPool *pmp,
					CExpressionHandle &exprhdl,
					CDistributionSpec *pdsRequired,
					ULONG ulChildIndex,
					DrgPdp *pdrgpdpCtxt,
					ULONG ulOptReq
				)
				const;

	}; // class CPhysicalSerialUnionAll

}

#endif // !GPOPT_CPhysicalSerialUnionAll_H

// EOF
