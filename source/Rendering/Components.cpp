//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#include "Components.h"
#include "Graphics/GfxCommandContext.h"


// Namespace Case_Engine
namespace Case_Engine
{

	void Mesh::Draw(GfxCommandContext* context) const
	{
		Draw(context, topology);
	}

	void Mesh::Draw(GfxCommandContext* context, GfxPrimitiveTopology topology) const
	{
		context->SetTopology(topology);
		context->SetVertexBuffer(vertex_buffer.get());
		if (index_buffer)
		{
			context->SetIndexBuffer(index_buffer.get());
			if (instance_buffer) context->SetVertexBuffer(instance_buffer.get(), 1);
			context->DrawIndexed(indices_count, instance_count, start_index_location, base_vertex_location, start_instance_location);
		}
		else
		{
			if (instance_buffer) context->SetVertexBuffer(instance_buffer.get(), 1);
			context->Draw(vertex_count, instance_count, start_vertex_location, start_instance_location);
		}
	}

}

