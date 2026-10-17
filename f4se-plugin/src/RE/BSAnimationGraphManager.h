#pragma once

namespace RE
{
	class BShkbAnimationGraph;

	// CommonLibF4's BSAnimationGraphManager has member types that are only forward declared
	// (AnimVariableCacheInfo, BSAnimationGraphChannel), so a BSTSmartPointer<BSAnimationGraphManager> that
	// can delete the manager does not compile. This policy only counts references (the holder always keeps
	// its own reference while the plugin looks at the manager).
	template <class T>
	struct NoDeleteRefCount
	{
		static void Acquire(T* a_ptr) { a_ptr->IncRef(); }
		static void Release(T* a_ptr) { (void)a_ptr->DecRef(); }
	};

	using BSAnimationGraphManagerPtr = BSTSmartPointer<BSAnimationGraphManager, NoDeleteRefCount>;

	// Gets the holder's graph manager as a reference counted (never deleting) pointer.
	inline BSAnimationGraphManagerPtr GetAnimationGraphManager(const IAnimationGraphManagerHolder* a_holder)
	{
		BSAnimationGraphManagerPtr result;
		if (a_holder) {
			a_holder->GetAnimationGraphManagerImpl(reinterpret_cast<BSTSmartPointer<BSAnimationGraphManager>&>(result));
		}
		return result;
	}

	namespace BShkbUtils
	{
		class GraphTraverser
		{
		public:
			GraphTraverser(int a_unkMaybeGraphType, hkbNode* a_rootNode) {
				Ctor(a_unkMaybeGraphType, a_rootNode);
			}

			~GraphTraverser() {
				DtorTree(&BSTBTreePolicy_tree[0]);
				DtorArena(&BSTObjectArena_arena[0]);
			}

			hkbNode* Next()
			{
				using func_t = decltype(&GraphTraverser::Next);
				static REL::Relocation<func_t> func{ REL::VariantID{ 849404, 2258082 } };
				return func(this);
			}

			uint8_t BSTObjectArena_arena[64];
			uint8_t BSTBTreePolicy_tree[56];

		private:
			void Ctor(int a_unk, hkbNode* a_rootNode) {
				using func_t = decltype(&GraphTraverser::Ctor);
				static REL::Relocation<func_t> func{ REL::VariantID{ 424303, 2258080 } };
				return func(this, a_unk, a_rootNode);
			}

			static void DtorTree(uint8_t* a_tree) {
				using func_t = decltype(&GraphTraverser::DtorTree);
				static REL::Relocation<func_t> func{ REL::VariantID{ 16523, 2258107 } };
				return func(a_tree);
			}

			static void DtorArena(uint8_t* a_arena)
			{
				using func_t = decltype(&GraphTraverser::DtorArena);
				static REL::Relocation<func_t> func{ REL::VariantID{ 1530664, 2258108 } };
				return func(a_arena);
			}
		};
	}

	class BShkbAnimationGraph;

	class BShkbVisitor
	{
	public:
		static constexpr auto VTABLE{ VTABLE::BSAnimGraphVisit__BShkbVisitor };

		BShkbVisitor()
		{
			REX::EMPLACE_VTABLE(this);
			character = nullptr;
			unk01 = 1;
			unk02 = 0;
		}

		virtual BSVisitControl TraverseAnimationGraphNodes(hkbNode* a_node) {
			using func_t = decltype(&BShkbVisitor::TraverseAnimationGraphNodes);
			static REL::Relocation<func_t> func{ REL::VariantID{ 1065709, 2257415 } };
			return func(this, a_node);
		}
		virtual BSVisitControl Visit(hkbNode*) = 0;
		virtual BSVisitControl InitVisit() { return RE::BSVisitControl::kContinue; }

		hkbCharacter* character;
		int32_t unk01;
		BSFixedString unk02;
	};

	struct hkbGraphInformation : public BShkbVisitor
	{
		virtual BSVisitControl Visit(hkbNode*) override { return RE::BSVisitControl::kStop; }
	};

	struct hkbContext
	{
	public:

		hkbContext(hkbCharacter* a_char, void* a_physIntfc = nullptr, void* a_attchMngr = nullptr) {
			Ctor(a_char, a_physIntfc, a_attchMngr);
		}

		~hkbContext() {
			Dtor();
		}

		uint8_t unk01[104];

	private:
		void Ctor(hkbCharacter* a_char, void* a_physIntfc = nullptr, void* a_attchMngr = nullptr)
		{
			using func_t = decltype(&hkbContext::Ctor);
			static REL::Relocation<func_t> func{ REL::VariantID{ 1381136, 2258328 } };
			return func(this, a_char, a_physIntfc, a_attchMngr);
		}

		void Dtor() {
			using func_t = decltype(&hkbContext::Dtor);
			static REL::Relocation<func_t> func{ REL::VariantID{ 144578, 2258329 } };
			return func(this);
		}
	};

	class BShkbAnimationGraph
	{
	public:
		uint8_t pad00[0x1C8];
		hkbCharacter character;

		void VisitGraph(BShkbVisitor& a_visitor)
		{
			using func_t = decltype(&BShkbAnimationGraph::VisitGraph);
			static REL::Relocation<func_t> func{ REL::VariantID{ 194777, 2256830 } };
			return func(this, a_visitor);
		}

		hkbNode* getNodeClone(hkbNode* a_node) {
			using func_t = decltype(&BShkbAnimationGraph::getNodeClone);
			static REL::Relocation<func_t> func{ REL::VariantID{ 326555, 2260454 } };
			return func(this, a_node);
		}
	};
	static_assert(offsetof(BShkbAnimationGraph, character) == 0x1C8);
}
