#pragma once

namespace Graphucks
{
    class GraphucksComponent
	{
	public:
		GraphucksComponent(){};
		virtual ~GraphucksComponent(){};

	private:
		//copy semantics: Non copyable
        GraphucksComponent(const GraphucksComponent&); // not implemented
        GraphucksComponent& operator = (const GraphucksComponent&); // not implemented
	};
}