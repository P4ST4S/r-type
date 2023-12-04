/*
** EPITECH PROJECT, 2023
** R-Type
** File description:
** HandleEntity
*/

#include "HandleEntity.hpp"

HandleEntity::HandleEntity(Registry &registry, std::shared_ptr<RGraphic::IGraphic> &graphic) : _registry(registry), _graphic(graphic), _dyingEntities(), _bank()
{
    _bank[PLAYER1_IDLE] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER1_IDLE].path);
    _bank[PLAYER1_DEAD] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER1_DEAD].path);
    _bank[PLAYER1_DOWN] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER1_DOWN].path);
    _bank[PLAYER1_UP] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER1_UP].path);
    _bank[PLAYER2_IDLE] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER2_IDLE].path);
    _bank[PLAYER2_DEAD] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER2_DEAD].path);
    _bank[PLAYER2_DOWN] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER2_DOWN].path);
    _bank[PLAYER2_UP] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER2_UP].path);
    _bank[PLAYER3_IDLE] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER3_IDLE].path);
    _bank[PLAYER3_DEAD] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER3_DEAD].path);
    _bank[PLAYER3_DOWN] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER3_DOWN].path);
    _bank[PLAYER3_UP] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER3_UP].path);
    _bank[PLAYER1_MISSILE] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER1_MISSILE].path);
    _bank[PLAYER2_MISSILE] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER2_MISSILE].path);
    _bank[PLAYER3_MISSILE] = _graphic->loadTexture(TexturesGlobal::BANK[PLAYER3_MISSILE].path);
    _bank[GHOST_BOSS_MISSILE] = _graphic->loadTexture(TexturesGlobal::BANK[GHOST_BOSS_MISSILE].path);
    _bank[BLIMP_GREEN_IDLE] = _graphic->loadTexture(TexturesGlobal::BANK[BLIMP_GREEN_IDLE].path);
    _bank[BLIMP_GREEN_ATTACK] = _graphic->loadTexture(TexturesGlobal::BANK[BLIMP_GREEN_ATTACK].path);
    _bank[BLIMP_PURPLE_IDLE] = _graphic->loadTexture(TexturesGlobal::BANK[BLIMP_PURPLE_IDLE].path);
    _bank[BLIMP_PURPLE_ATTACK] = _graphic->loadTexture(TexturesGlobal::BANK[BLIMP_PURPLE_ATTACK].path);
    _bank[BLUE_DEVIL_FLY] = _graphic->loadTexture(TexturesGlobal::BANK[BLUE_DEVIL_FLY].path);
    _bank[ENEMY_BULLET] = _graphic->loadTexture(TexturesGlobal::BANK[ENEMY_BULLET].path);
    _bank[ENEMY_PINK_BULLET] = _graphic->loadTexture(TexturesGlobal::BANK[ENEMY_PINK_BULLET].path);
    _bank[GHOST_BOSS_IDLE] = _graphic->loadTexture(TexturesGlobal::BANK[GHOST_BOSS_IDLE].path);
    _bank[GHOST_BOSS_ATTACK] = _graphic->loadTexture(TexturesGlobal::BANK[GHOST_BOSS_ATTACK].path);
    _bank[GHOST_BOSS_DEAD] = _graphic->loadTexture(TexturesGlobal::BANK[GHOST_BOSS_DEAD].path);
    _bank[SMOKE_EXPLOSION] = _graphic->loadTexture(TexturesGlobal::BANK[SMOKE_EXPLOSION].path);
    _bank[EXPLOSION] = _graphic->loadTexture(TexturesGlobal::BANK[EXPLOSION].path);
}

HandleEntity::~HandleEntity()
{
}

std::size_t HandleEntity::createEntity(NewEntities_t newEntity)
{
    // std::cout << newEntity._id << std::endl;

    std::size_t id = this->_registry.spawnEntity();
    this->_entities[newEntity._id] = id;

    Entity entity = this->_registry.entityFromIndex(id);

    this->_registry.addComponent<PositionComponent>(entity, PositionComponent(50, 50));
    this->_registry.addComponent<InterpolationComponent>(entity, InterpolationComponent());

    int defaultSprite = 0;
    int deathSprite = 0;
    int moveUpSprite = 0;
    int moveDownSprite = 0;
    int attackSprite = 0;

    TextureComponent &texture = *_registry.addComponent<TextureComponent>(entity, TextureComponent());
    switch (newEntity._type)
    {
    case (PLAYER1_ID):
        defaultSprite = PLAYER1_IDLE;
        deathSprite = PLAYER1_DEAD;
        moveUpSprite = PLAYER1_DOWN;
        moveDownSprite = PLAYER1_UP;
        attackSprite = PLAYER1_IDLE;
        break;

    case (PLAYER2_ID):
        defaultSprite = PLAYER2_IDLE;
        deathSprite = PLAYER2_DEAD;
        moveUpSprite = PLAYER2_DOWN;
        moveDownSprite = PLAYER2_UP;
        attackSprite = PLAYER2_IDLE;
        break;

    case (PLAYER3_ID):
        defaultSprite = PLAYER3_IDLE;
        deathSprite = PLAYER3_DEAD;
        moveUpSprite = PLAYER3_DOWN;
        moveDownSprite = PLAYER3_UP;
        attackSprite = PLAYER3_IDLE;
        break;

    case (MOB_ID):
        defaultSprite = BLIMP_GREEN_IDLE;
        deathSprite = SMOKE_EXPLOSION;
        moveUpSprite = BLIMP_GREEN_IDLE;
        moveDownSprite = BLIMP_GREEN_IDLE;
        attackSprite = BLIMP_GREEN_ATTACK;
        break;

    case (PURPLE_BLIMP):
        defaultSprite = BLIMP_PURPLE_IDLE;
        deathSprite = SMOKE_EXPLOSION;
        moveUpSprite = BLIMP_PURPLE_IDLE;
        moveDownSprite = BLIMP_PURPLE_IDLE;
        attackSprite = BLIMP_PURPLE_ATTACK;
        break;

    case (MISSILE_ID):
        defaultSprite = ENEMY_BULLET;
        deathSprite = EXPLOSION;
        moveUpSprite = ENEMY_BULLET;
        moveDownSprite = ENEMY_BULLET;
        attackSprite = ENEMY_BULLET;
        break;

    case (PLAYER1_MISSILE_ID):
        defaultSprite = PLAYER1_MISSILE;
        deathSprite = EXPLOSION;
        moveUpSprite = PLAYER2_MISSILE;
        moveDownSprite = PLAYER1_MISSILE;
        attackSprite = PLAYER1_MISSILE;
        break;

    case (PLAYER2_MISSILE_ID):
        defaultSprite = PLAYER2_MISSILE;
        deathSprite = EXPLOSION;
        moveUpSprite = PLAYER2_MISSILE;
        moveDownSprite = PLAYER2_MISSILE;
        attackSprite = PLAYER2_MISSILE;
        break;

    case (PLAYER3_MISSILE_ID):
        defaultSprite = PLAYER3_MISSILE;
        deathSprite = EXPLOSION;
        moveUpSprite = PLAYER3_MISSILE;
        moveDownSprite = PLAYER3_MISSILE;
        attackSprite = PLAYER3_MISSILE;
        break;

    case (MOB_MISSILE_ID):
        defaultSprite = ENEMY_BULLET;
        deathSprite = EXPLOSION;
        moveUpSprite = ENEMY_BULLET;
        moveDownSprite = ENEMY_BULLET;
        attackSprite = ENEMY_BULLET;
        break;

    case (GHOST_BOSS_MISSILE_ID):
        defaultSprite = GHOST_BOSS_MISSILE;
        deathSprite = EXPLOSION;
        moveUpSprite = ENEMY_BULLET;
        moveDownSprite = ENEMY_BULLET;
        attackSprite = ENEMY_BULLET;
        break;

    case (GHOST_BOSS_ID):
        defaultSprite = GHOST_BOSS_IDLE;
        deathSprite = GHOST_BOSS_DEAD;
        moveUpSprite = GHOST_BOSS_IDLE;
        moveDownSprite = GHOST_BOSS_IDLE;
        attackSprite = GHOST_BOSS_ATTACK;
        break;

    default:
        defaultSprite = ENEMY_BULLET;
        deathSprite = EXPLOSION;
        moveUpSprite = ENEMY_BULLET;
        moveDownSprite = ENEMY_BULLET;
        attackSprite = ENEMY_BULLET;
        break;
    }

    texture.addTexture(DEFAULT_ACTION, _bank[defaultSprite], TexturesGlobal::BANK[defaultSprite].dimension, TexturesGlobal::BANK[defaultSprite].nframes, 1);
    texture.addTexture(DEATH_ACTION, _bank[deathSprite], TexturesGlobal::BANK[deathSprite].dimension, TexturesGlobal::BANK[deathSprite].nframes, 1);
    texture.addTexture(MOVE_UP_ACTION, _bank[moveUpSprite], TexturesGlobal::BANK[moveUpSprite].dimension, TexturesGlobal::BANK[moveUpSprite].nframes, 1);
    texture.addTexture(MOVE_DOWN_ACTION, _bank[moveDownSprite], TexturesGlobal::BANK[moveDownSprite].dimension, TexturesGlobal::BANK[moveDownSprite].nframes, 1);
    texture.addTexture(ATTACK_ACTION, _bank[attackSprite], TexturesGlobal::BANK[attackSprite].dimension, TexturesGlobal::BANK[attackSprite].nframes, 1);
    return id;
}

std::size_t HandleEntity::replaceEntity(NewEntities_t newEntity)
{
    Entity oldEntity = this->_registry.entityFromIndex(_entities[newEntity._id]);
    this->_registry.killEntity(oldEntity);

    return this->createEntity(newEntity);
}

void HandleEntity::destroyEntity(std::size_t id)
{
    SparseArray<TextureComponent> &texture = this->_registry.getComponents<TextureComponent>();

    if (_entities.find(id) != _entities.end())
    {
        Entity entity = this->_registry.entityFromIndex(_entities[id]);
        if (texture[entity].has_value())
        {
            _dyingEntities.push_back(entity);
            texture[entity]->playTexture(DEATH_ACTION);
            this->_entities.erase(id);
        }
    }
}

double HandleEntity::moveEntity(MoveEntitiesDyn_s &moveEntities, std::chrono::duration<double> &timeStamp)
{
    double coef = 0;
    SparseArray<InterpolationComponent> &interpolation = this->_registry.getComponents<InterpolationComponent>();
    SparseArray<TextureComponent> &texture = this->_registry.getComponents<TextureComponent>();

    if (_entities.find(moveEntities._id) != _entities.end())
    {
        Entity entity = this->_registry.entityFromIndex(_entities[moveEntities._id]);
        if (interpolation[entity].has_value() && texture[entity].has_value())
        {
            if (interpolation[entity]->getPosition()._y - moveEntities._y > 0)
                texture[_entities[moveEntities._id]]->setDefaultTexture(MOVE_DOWN_ACTION);
            else if (interpolation[entity]->getPosition()._y - moveEntities._y < 0)
                texture[_entities[moveEntities._id]]->setDefaultTexture(MOVE_UP_ACTION);
            else
                texture[_entities[moveEntities._id]]->setDefaultTexture(DEFAULT_ACTION);
            interpolation[entity]->setMoveEntityBody(moveEntities);
            interpolation[entity]->setStartTime(timeStamp);
            coef = interpolation[entity]->getInterpolationCoefficient(std::chrono::high_resolution_clock::now());
        }
    }
    return (coef);
}

void HandleEntity::UpdatedyingEntities()
{
    for (auto it = _dyingEntities.begin(); it != _dyingEntities.end();)
    {
        Entity entity = this->_registry.entityFromIndex(*it);
        SparseArray<TextureComponent> &texture = this->_registry.getComponents<TextureComponent>();
        if (texture[entity]->getCurrentId() != DEATH_ACTION){
            this->_registry.killEntity(entity);
            it = _dyingEntities.erase(it);
        } else
            it++;
    }
}

bool HandleEntity::entityExist(std::size_t id) const
{
    if (_entities.find(id) == _entities.end())
        return (true);
    return (false);
}

std::size_t HandleEntity::getEntity(std::size_t id) const
{
    if (!entityExist(id))
        return (0);
    return (_entities.at(id));
}

void HandleEntity::packetsInterpreter(std::deque<Package> &packets)
{
    std::deque<Package> tempPkgs = PackageParser::extractPackages(packets, {NEW_ENTITIES, DESTROY_ENTITIES, MOVE_ENTITIES});
    Package pkg;

    // for (size_t i = 0; i < tempPkgs.size(); ++i) {
    //     std::cout << tempPkgs[i].getHeader().get()._commandType << std::endl;
    // }
    while (!tempPkgs.empty())
    {
        pkg = tempPkgs.front();
        tempPkgs.pop_front();
        switch (pkg.getHeader().get()._commandType)
        {
        case NEW_ENTITIES:
            this->updateNewEntities(pkg);
            break;
        case DESTROY_ENTITIES:
            this->updateDestroyEntities(pkg);
            break;
            ;
        case MOVE_ENTITIES:
            this->updateMoveEntities(pkg);
            break;
        }
    }
}

void HandleEntity::updateNewEntities(Package &package)
{
    DynamicBody<NewEntities_t> &newEntitiesBody = package.getBody<DynamicBody<NewEntities_t>>();

    for (size_t i = 0; i < newEntitiesBody.get().size(); i++)
        createEntity(newEntitiesBody.get()[i]);
}

void HandleEntity::updateDestroyEntities(Package &package)
{
    DynamicBody<DestroyEntities_t> &destroyEntitiesBody = package.getBody<DynamicBody<DestroyEntities_t>>();

    for (size_t i = 0; i < destroyEntitiesBody.get().size(); i++)
        destroyEntity(destroyEntitiesBody.get()[i]._id);
}

void HandleEntity::updateMoveEntities(Package &package)
{
    double coef = 0;
    HybridBody<MoveEntitiesSta_t, MoveEntitiesDyn_s> &moveEntitiesBody = package.getBody<HybridBody<MoveEntitiesSta_t, MoveEntitiesDyn_s>>();
    std::chrono::duration<double> timeNow(moveEntitiesBody.get().first.get()._timeStamp);
    std::chrono::duration<double> timeStamp;

    if (this->_firstMovePkg)
        this->_startTime = std::chrono::high_resolution_clock::now().time_since_epoch() - timeNow;
    timeStamp = this->_startTime + timeNow;
    // std::cout << "timeNow: " << timeNow.count() << " timeStamp: " << timeStamp.count() << std::endl;
    for (size_t i = 0; i < moveEntitiesBody.get().second.elemCount(); i++)
        coef = moveEntity(moveEntitiesBody.get().second.get()[i], timeStamp);
    // std::cout << "coef: " << coef << std::endl;
    if (coef < 0) {
        this->_startTime += std::chrono::duration<double>(coef * 0.1);
    }
    this->_lastTime = timeNow;
    this->_firstMovePkg = false;
}

void HandleEntity::resetFirstMovePkg()
{
    this->_firstMovePkg = true;
}
