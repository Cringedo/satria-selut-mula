// MeleeAttackStrategy.hpp
#include <self/AttackStrategy.hpp>
#include <self/Entity.hpp>

class MeleeAttackStrategy : public IAttackStrategy {
public:
    void Execute(Entity& self, Entity& target) override; // Definition in .cpp
};

void MeleeAttackStrategy::Execute(Entity& self, Entity& target) {
    TraceLog(LOG_INFO, "%s performs a melee attack!", self.GetName().c_str());
    
}