#include "CommandQuestLog.h"
#include "Menu.h"

namespace Dusk
{

CommandQuestLog::CommandQuestLog(const QuestLogOperation op)
{
  m_Operation = op;
}

CommandQuestLog::~CommandQuestLog()
{
  //empty
}

bool CommandQuestLog::execute(Dusk::Scene* scene, int count)
{
  switch (m_Operation)
  {
    case qloToggle:
         //toggles visibility of QuestLog menu
         Menu::GetSingleton().showQuestLog(!Menu::GetSingleton().isQuestLogVisible());
         break;
    case qloNext:
         Menu::GetSingleton().nextQuestLogPage();
         break;
    case qloPrev:
         Menu::GetSingleton().previousQuestLogPage();
         break;
    default: //should never happen
         throw 42;
  }//swi
  return true;
}

} //namespace
