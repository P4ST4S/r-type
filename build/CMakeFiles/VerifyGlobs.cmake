# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.27
cmake_policy(SET CMP0009 NEW)

# CPP_SOURCES at lib/Clock/CMakeLists.txt:7 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/Clock/src/*.cpp")
set(OLD_GLOB
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/Clock/src/Clock.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/build/CMakeFiles/cmake.verify_globs")
endif()

# CPP_SOURCES at lib/CrossPlatformNetwork/CMakeLists.txt:7 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/CrossPlatformNetwork/src/*.cpp")
set(OLD_GLOB
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/CrossPlatformNetwork/src/ANetwork.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/CrossPlatformNetwork/src/Buffer.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/CrossPlatformNetwork/src/Client.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/CrossPlatformNetwork/src/CpnUtils.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/CrossPlatformNetwork/src/ReceptionFunctions.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/CrossPlatformNetwork/src/Server.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/build/CMakeFiles/cmake.verify_globs")
endif()

# CPP_SOURCES at lib/GameEngine/CMakeLists.txt:7 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/*.cpp")
set(OLD_GLOB
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Architecture/ASystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Architecture/EventsWorker.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Architecture/Registry.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Components/DamageHitboxComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Components/IdentityComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Components/InterpolationComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Components/LifeHitboxComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Components/PositionComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Components/VelocityComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Geometry/Collision.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Systems/HealthUpdatingSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Systems/InterpolationMoveSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/GameEngine/src/Systems/InterpolationSystem.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/build/CMakeFiles/cmake.verify_globs")
endif()

# CPP_SOURCES at lib/NetworkProtocol/CMakeLists.txt:7 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/*.cpp")
set(OLD_GLOB
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/Body/BodyFactory.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/Body/EmptyBody.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/Header/Header.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/Package/ClientPackageHandler.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/Package/Package.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/Package/PackageFactory.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/Package/PackageParser.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/NetworkProtocol/src/Package/ServerPackageHandler.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/build/CMakeFiles/cmake.verify_globs")
endif()

# CPP_SOURCES at lib/Raygui/CMakeLists.txt:9 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/Raygui/src/*.c")
set(OLD_GLOB
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/Raygui/src/raygui.c"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/build/CMakeFiles/cmake.verify_globs")
endif()

# CPP_SOURCES at lib/RaylibGraphic/CMakeLists.txt:7 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/RaylibGraphic/src/*.cpp")
set(OLD_GLOB
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/lib/RaylibGraphic/src/RaylibGraphic.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/build/CMakeFiles/cmake.verify_globs")
endif()

# CPP_SOURCES at CMakeLists.txt:8 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/*.cpp")
set(OLD_GLOB
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Components/ButtonComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Components/CheckboxComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Components/DropDownButtonComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Components/InputBoxComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Components/MusicComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Components/SliderComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Components/TextComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Components/TextureComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/EventListeners/InputEventListener.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/GlobalSettings.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Scenes/ConnectToServerMenuScene.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Scenes/ConnectingToServerMenuScene.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Scenes/GameLoadingScene.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Scenes/GameScene.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Scenes/MainMenuScene.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Scenes/ServerLobbyMenuScene.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Scenes/SettingsMenuScene.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Scenes/TitleMenuScene.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/ButtonSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/CheckboxSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/DropDownButtonSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/EndFrameSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/InputBoxSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/SceneManagerSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/SliderSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/StartFrameSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/TextSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/Systems/TextureSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/TextureBank.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/Graphic/handleEntity.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Client/main.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/build/CMakeFiles/cmake.verify_globs")
endif()

# CPP_SERV_SOURCES at CMakeLists.txt:11 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/*.cpp")
set(OLD_GLOB
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ClientHandler.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Components/PlayerEventsComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Components/TagComponents/BossComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Components/TagComponents/BulletComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Components/TagComponents/MobComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Components/TagComponents/PlayerComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Components/WeaponComponent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Events/KillEntityEvent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Events/NewEntityEvent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/Events/PartyEndEvent.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/RTypeServer/RTypeServer.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/RTypeServer/RunGame.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/RTypeServer/RunLobby.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleAuthenticationSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleBossAISystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleConnectionSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleDisconnectionPackageSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleGameStatusSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleInformationPackageSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleLoadStatusSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleLobbyStatusSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleMobAISystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HandleUnloadStatusSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/HitboxSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/MissileSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/ReceiveNewDataSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/SendDestroyEntitiesPackageSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/SendMoveEntitiesPackageSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/SendNewEntitiesPackageSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/UpdateEntitiesMovementSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/ServerSystems/WaveSystem.cpp"
  "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/src/Server/main.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/pastas/delivery/B-CPP-500-PAR-5-1-rtype-ethan.nguyen/build/CMakeFiles/cmake.verify_globs")
endif()
