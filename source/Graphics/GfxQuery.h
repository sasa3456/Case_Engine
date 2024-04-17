//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <d3d11.h>


// Namespace Case_Engine
namespace Case_Engine
{
	enum class QueryType
	{
		Event = 0,
		Occlusion,
		Timestamp,
		TimestampDisjoint,
		PipelineStatistics,
		OcclusionPredicate
	};

	class GfxDevice;
	class GfxQuery
	{
	public:
		GfxQuery(GfxDevice* gfx, QueryType type);

		operator ID3D11Query*() const 
		{
			return query.Get();
		}

	private:
		ArcPtr<ID3D11Query> query;
	};

	struct QueryDataTimestampDisjoint
	{
		uint64_t frequency;
		bool disjoint;
	};

	struct QueryDataPipelineStatistics
	{
		uint64_t IAVertices;
		uint64_t IAPrimitives;
		uint64_t VSInvocations;
		uint64_t GSInvocations;
		uint64_t GSPrimitives;
		uint64_t CInvocations;
		uint64_t CPrimitives;
		uint64_t PSInvocations;
		uint64_t HSInvocations;
		uint64_t DSInvocations;
		uint64_t CSInvocations;
	};
}