/*
** EPITECH PROJECT, 2023
** B-CPP-500-PAR-5-1-rtype-ethan.nguyen
** File description:
** SceneManagerSystem
*/

#include "SceneManagerSystem.hpp"

SceneManagerSystem::SceneManagerSystem(Registry &registry, std::shared_ptr<RGraphic::IGraphic> lib) : 
_currentScene("None"), _lib(lib), _registry(registry), _entityHandler(registry, lib)
{
}

SceneManagerSystem::~SceneManagerSystem()
{
}

void SceneManagerSystem::operator()(Registry &registry)
{
    if (!_sceneList.empty())
        getScene(_currentScene).run(*this);
}

bool SceneManagerSystem::sceneNameExist(const std::string &sceneName)
{
    return (_sceneList.find(sceneName) != _sceneList.end());
}

IScene &SceneManagerSystem::getScene(const std::string &sceneName)
{
    return (*(_sceneList[_currentScene].first.get()));
}

void SceneManagerSystem::changeScene(Registry &registry, const std::string &sceneName)
{
    if (sceneNameExist(sceneName)) {
        if (_currentScene != "None")
            getScene(_currentScene).unloadScene(*this);
        _currentScene = sceneName;
        getScene(_currentScene).loadScene(*this);
    }
}

ClientPackageHandler &SceneManagerSystem::getClientPkgHandler(void)
{
    return (this->_pkgHandler);
}

HandleEntity &SceneManagerSystem::getEntityHandler(void)
{
    return (this->_entityHandler);
}

RGraphic::RMusic &SceneManagerSystem::getMusicTheme(void)
{
    return (this->_musicTheme);
}