#include <boost/test/unit_test.hpp>
#include "../../src/shared/state.h"
#include "../../src/shared/state/UnitFactory.h"
#include "../../src/shared/engine.h"
#include "../../src/shared/ai.h"


using namespace state;
using namespace engine;
using namespace ai;

#define Player1_ID 1
#define Player2_ID 2
#define UNIT_LAYER_ID  2
#define BUILDING_LAYER_ID  1

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

//TESTS DE GAMEINSTANCE

BOOST_AUTO_TEST_CASE(GameInstance_Constructor1_Test){
  //Test du premier constructeur de GameInstance
  GameInstance gi = GameInstance("a dwarf", DWARF);
  //Vérification des attributs attribués par le constructeur
  BOOST_CHECK_EQUAL(gi.getName(), "a dwarf"); //nom
  BOOST_CHECK_EQUAL(gi.getID(), 0); //ID
  BOOST_CHECK_EQUAL(gi.getTypeID(), DWARF); //type DWARF
  BOOST_CHECK_EQUAL(gi.getX(), 0); //position X
  BOOST_CHECK_EQUAL(gi.getY(), 0); //position Y
}

BOOST_AUTO_TEST_CASE(GameInstance_Constructor2_Test){
  //Test du deuxième constructeur de GameInstance
  GameInstance gi = GameInstance(BAT);
  //Vérification des attributs attribués par le constructeur
  BOOST_CHECK_EQUAL(gi.getID(), 1);//ID
  BOOST_CHECK_EQUAL(gi.getTypeID(), BAT);//type BAT
}


//TEST DE UNITINSTANCE


//TESTS DE STATE
BOOST_AUTO_TEST_CASE(State_findPlayerAllies_Test){
  //Test de la méthode findPlayerAllies de State

  State state;
  UnitInstance unit1 = UnitInstance(BAT, Player1_ID);

  
}

BOOST_AUTO_TEST_CASE(GameInstanceTest)
  {
    GameInstance gi1 ("GI1", (GameInstanceTypeID) 1);
    //BOOST_CHECK_EQUAL(gi1.getID(), 1);
    BOOST_CHECK_EQUAL(gi1.getX(), 0);
    BOOST_CHECK_EQUAL(gi1.getY(), 0);
    BOOST_CHECK_EQUAL(gi1.getName(), "GI1");
    GameInstance gi2 ((GameInstanceTypeID) 1);
    //BOOST_CHECK_EQUAL(gi2.getID(), 2);

    std::vector<int> newPos = {2,3};
    gi1.assignPosition(1,0);
    BOOST_CHECK_EQUAL(gi1.getX(), 1);
    BOOST_CHECK_EQUAL(gi1.getY(), 0);
    gi2.assignPosition(newPos);
    BOOST_CHECK_EQUAL(gi2.getX(), 2);
    BOOST_CHECK_EQUAL(gi2.getY(), 3);

    gi1.select();
    BOOST_CHECK_EQUAL(gi1.isSelected(), true);
    gi1.unselect();
    BOOST_CHECK_EQUAL(gi1.isSelected(), false);

    gi1.setPlayerID(1); 
    BOOST_CHECK_EQUAL(gi1.getPlayerID(), 1);
    gi2.setPlayerID(2);
    BOOST_CHECK_EQUAL(gi2.getPlayerID(), 2);
    
    State stateO;

    GameInstanceManager *ugim = new GameInstanceManager("units", 0);
    BOOST_CHECK_EQUAL(ugim->getID(), 0);
    BOOST_CHECK_EQUAL(ugim->getSize(), 0);
    
    UnitFactory uf;
    UnitInstance *bat1 = (UnitInstance*) uf.createGI(GameInstanceTypeID::BAT, Player1_ID);
    bat1->assignPosition(22,15);
    BOOST_CHECK_EQUAL(bat1->showHP(), 10);

    
    
    stateO.addGIM("units", ugim);
    stateO._GImanagers["units"]->add(bat1);
    BOOST_CHECK_EQUAL(ugim->getSize(), 1);
    ugim->selectObjective(bat1->getPosition());
    BOOST_CHECK_EQUAL(bat1, stateO.getObjective());

    int test_id = stateO.getGI(bat1->getX(), bat1->getY())->getID();
    BOOST_CHECK_EQUAL(test_id, bat1->getID());

    ugim->remove(bat1);
    BOOST_CHECK_EQUAL(ugim->getSize(), 0);


    BOOST_CHECK_EQUAL(stateO.getObjective()->getID(), bat1->getID());
    
  }



BOOST_AUTO_TEST_CASE(TestStateEngine)
{
  State state;
  state.init();

  //création dest joueurs 
  Player p1("player 1","red");
  p1.setID(1);
  Player p2("player 2","blues");
  p2.setName("Player_2");
  BOOST_CHECK_EQUAL(p2.getName(), "Player_2");
  p2.setID(2);
  p1.credit(100);

  state.Players.push_back(&p1);
  state.Players.push_back(&p2);

  BOOST_CHECK_EQUAL(state.Players.size(), 2);

  BOOST_CHECK_EQUAL(state.GetActivePlayer()->getID(), 1);

  //création des unités
  GameInstance *dwarf1 = new GameInstance("Dwarf_1", GameInstanceTypeID::DWARF);
  dwarf1->assignPosition(12,15);

  //BOOST_REQUIRE_EQUAL(dwarf1->getID(), 3);
  dwarf1->setPlayerID(1);

  GameInstance *dwarf2 = new GameInstance("Dwarf_2", GameInstanceTypeID::DWARF);
  dwarf2->assignPosition(12,16);
  BOOST_REQUIRE_EQUAL(dwarf2->getX(), 12);
  dwarf2->setPlayerID(2);

  GameInstance *dwarf3 = new GameInstance("Dwarf_3", GameInstanceTypeID::DWARF);
  dwarf3->assignPosition(12,17);
  BOOST_REQUIRE_EQUAL(dwarf2->getPlayerID(), 2);
  dwarf3->setPlayerID(2);

  //création du bâtiment
  GameInstanceManager *buildingGIM = new GameInstanceManager("Building's Manager", 2, ""); // macro BUILDING_LAYER_ID
  GameInstance *HQ1 = new GameInstance("HeadQuarter", GameInstanceTypeID::HEADQUARTER);
  HQ1->setPlayerID(1);
  HQ1->assignPosition(3,3);
  buildingGIM->add(HQ1);

  GameInstanceManager *unitGIM = new GameInstanceManager("Unit's Manager", 3, ""); //macro UNIT_LAYER_ID
  unitGIM->add(dwarf1);
  unitGIM->add(dwarf2);

  state.addGIM("buildings", buildingGIM);
  state.addGIM("units", unitGIM);

  BOOST_CHECK_EQUAL(state._GImanagers.size(), 2);

  state.addGI("units", dwarf3);

  BOOST_CHECK_EQUAL(unitGIM->getGameInstances().size(), 3);

  //Partie engine
  Engine engine(&state);
  auto selecDwarf1 = std::make_shared<SelectionCommand>(12, 15);

  //auto attackDwarf1 = std::make_shared<AttackCommand>();
  auto moveDwarf1 = std::make_shared<MoveCommand>(13, 15);
  auto selecbuild = std::make_shared<SelectionCommand>(3, 3);
  auto buildDwarf1 = std::make_shared<BuildUnitCommand>(HQ1->getID(), GameInstanceTypeID::DWARF);

  //test de la séléction
  engine.addCommand(selecDwarf1);
  engine.processCommands();
  BOOST_CHECK_EQUAL(state.getSource()->getID(), dwarf1->getID());
/*
  //test du déplacement 
  engine.addCommand(moveDwarf1);
  engine.processCommands();
  BOOST_CHECK_EQUAL(state.getSource()->getX(), 13);
*/
  //test de sélection du bâtiment
  engine.addCommand(selecbuild);
  engine.processCommands();
  BOOST_CHECK_EQUAL(state.getSource()->getID(), HQ1->getID());

  //test de la production d'unité
  engine.addCommand(buildDwarf1);
  engine.processCommands();
  BOOST_CHECK_EQUAL(state._GImanagers["units"]->getGameInstances().size(), 4);
  
  BOOST_CHECK_EQUAL(state.GetActivePlayer()->balance(), 50);

  //test de la commande d'attaque 

  state::UnitFactory *unitfact=new state::UnitFactory();
  state::GameInstance *dw1=unitfact->createGI(state::DWARF,1);
  state::GameInstance *dw2=unitfact->createGI(state::DWARF,2);
  state::GameInstanceManager *gimTest = new state::GameInstanceManager("units", 1);
  dw1->assignPosition(0, 1);
  dw2->assignPosition(1, 1);
 
  gimTest->add(dw1);
  gimTest->add(dw2);
  gimTest->selectSource(dw1->getPosition(),1);
  gimTest->selectObjective(dw2->getPosition());

    // test instance state  
  state::State state2;
  state.addGIM("units",gimTest);


    //test du déplacement moussa 
  state.selectSource(dw1->getPosition());
  state.selectSource(std::vector<int>{1,2});
  BOOST_CHECK_EQUAL(state.getPrevSelect()[0], 1);
  BOOST_CHECK_EQUAL(state.getPrevSelect()[1], 2);
  BOOST_CHECK_EQUAL(state.getSource()->getPosition()[0], dw1->getPosition()[0]);
  BOOST_CHECK_EQUAL(state.getSource()->getPosition()[1], dw1->getPosition()[1]);
   // state.selectSource(dw2->getPosition());
  
  //auto moveDwarftest = std::make_shared<MoveCommand>(1, 2);
  auto moveDwarftest = std::make_shared<MoveCommand>(1, 2);
  engine.addCommand(moveDwarftest);
  engine.processCommands();
  BOOST_CHECK_EQUAL(state.getSource()->getX(), 1);
  //engine::AttackCommand* attck = new engine::AttackCommand();
  //attck->process(state2);
  //state::UnitInstance* ennemy= (state::UnitInstance*)dw2;
  //BOOST_CHECK_EQUAL(ennemy->showHP(), 0);

  //test de fin de jeu
  //BOOST_CHECK_EQUAL(state.isOver, false);
  //BOOST_CHECK_EQUAL(state.WinnerID(), -1); //la méthode retourne -1 si persnne n'a gagné

}

BOOST_AUTO_TEST_CASE(TetsAiEngine){
  
  #define Player_ID 1
  #define AI_ID 2
 
  //Initialisation d'un état du jeu
  State state;
  Player* player = new Player("player", "red", Player1_ID);
  Player* ai = new Player("ai", "blue" ,AI_ID);

  player->credit(200);
  ai->credit(200);

  state.Players.push_back(player);
  state.Players.push_back(ai);

  GameInstance *dwarfPlayer = new GameInstance("Dwarf_Player", GameInstanceTypeID::DWARF);
  dwarfPlayer->assignPosition(12,15);
  dwarfPlayer->setPlayerID(Player_ID);

  GameInstance *dwarfAI = new GameInstance("Dwarf_AI", GameInstanceTypeID::DWARF);
  dwarfAI->assignPosition(12,16);
  int initialXPosition = dwarfAI->getX();
  dwarfAI->setPlayerID(AI_ID);

  GameInstanceManager *unitGIM = new GameInstanceManager("Unit's Manager", UNIT_LAYER_ID);
  state._GImanagers["units"] = unitGIM;
  unitGIM->add(dwarfPlayer);
  unitGIM->add(dwarfAI);

  //state.addGIM("units",unitGIM);

  //Création du moteur du jeu et instanciation de l'ia
  Engine engine(&state);
  RandomAI randomAI(16,16);

  //Génération des commandes de l'IA aléatoire
  randomAI.GenCommands(engine, state, AI_ID);
  
  engine.processCommands();
  int newXPosition = dwarfAI->getX();
  
  
  //BOOST_CHECK_PREDICATE(std::not_equal_to<int>(), (initialXPosition) (newXPosition));
  //BOOST_CHECK_PREDICATE(std::not_equal_to<int>(), (dwarfAI->getID()) (dwarfPlayer->getID()));
  
}

BOOST_AUTO_TEST_CASE(BuildingFactoryTest){
  BuildingFactory bf;
  BuildingInstance* HQ = (BuildingInstance*) bf.createGI(GameInstanceTypeID::HEADQUARTER, Player1_ID);
  BOOST_CHECK(!HQ->isBeignCaptured());
  HQ->capturing(Player2_ID);
  HQ->addCaptureCounter();
  BOOST_CHECK(HQ->isBeignCaptured());
  HQ->addCaptureCounter();
  BOOST_CHECK(HQ->getPlayerID() == Player2_ID);
}

BOOST_AUTO_TEST_CASE(AttackCommandTest){
  State state;
  UnitFactory uf;
  GameInstanceManager* units = new GameInstanceManager("units", UNIT_LAYER_ID);
  state.addGIM("units", units); 
  //création des unités et assignement des positions
  UnitInstance* bat = (UnitInstance*) uf.createGI(BAT, Player1_ID);
  bat->assignPosition(0,0);
  state.addGI("units", bat);
  UnitInstance* cyclop = (UnitInstance*) uf.createGI(CYCLOPS, Player2_ID);
  cyclop->assignPosition(0,1);
  state.addGI("units", cyclop);
  //sélection de la cible et de la source
  state.selectSource(cyclop->getPosition());
  state.selectSource(bat->getPosition());
  //state.selectObjective(bat->getPosition());
  //vérification de la taille du GIM des unités
  BOOST_CHECK(bat->isDead()==false);
 //test selection command moussa + Prevposition
  BOOST_CHECK(bat==state.getSource());
  BOOST_CHECK(cyclop==state.getObjective());
  BOOST_CHECK(state.getPrevSelect()[0]==bat->getPosition()[0]);
  BOOST_CHECK(state.getPrevSelect()[1]==bat->getPosition()[1]);

  
  //gestion de la commande
  Engine engine(&state);
  engine.addCommand(std::make_shared<AttackCommand>());
  engine.processCommands();
  //vérification de la mort du bat
  //BOOST_CHECK(bat->isDead()==true);
  //BOOST_CHECK(units->getGameInstances().size() == 1);
}

BOOST_AUTO_TEST_CASE(HeuristicAiTest){
  #define HEURISTIC_ID 1
  #define RANDOM_ID 2
  #define LENGTH 4
  #define HEIGHT 4

  State state;
  //Tests de l'IA Heuristic sur une petite map 4*4
  state::State heuristicAiState;
  //création de l'ia heuristique :
  ai::HeuristicAI heuristicAi(4,4);
  Player hAi("Heuristic AI", "red", HEURISTIC_ID);
  //création de l'ia random :
  ai::RandomAI randomAi(4,4);
  Player rAi("Random AI", "blue", RANDOM_ID);

  state.Players.push_back(&hAi);
  state.Players.push_back(&rAi);

  //création des unités et bâtiments
  GameInstanceManager* units = new GameInstanceManager("units", 1);
  GameInstanceManager* buildings = new GameInstanceManager("building", 2);
  state.addGIM("units", units); 
  state.addGIM("buildings", buildings);

}
