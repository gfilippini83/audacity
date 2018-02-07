/**********************************************************************

   Audacity: A Digital Audio Editor
   Audacity(R) is copyright (c) 1999-2009 Audacity Team.
   File License: wxwidgets

   BatchEvalCommand.h
   Dan Horgan

******************************************************************//**

\class BatchEvalCommand
\brief Given a string representing a command, pass it to the BatchCommands
system.

The eventual aim is to move the code from BatchCommands out into separate
command classes, but until this happens, BatchEvalCommand can act as a 'bridge'
to that system.

*//*******************************************************************/

#ifndef __BATCHEVALCOMMAND__
#define __BATCHEVALCOMMAND__

#include "Command.h"
#include "CommandType.h"
#include "../BatchCommands.h"

class BatchEvalCommandType final : public OldStyleCommandType
{
public:
   wxString BuildName() override;
   void BuildSignature(CommandSignature &signature) override;
   OldStyleCommandPointer Create(std::unique_ptr<CommandOutputTarget> &&target) override;
};

class BatchEvalCommand final : public CommandImplementation
{
public:
   BatchEvalCommand(OldStyleCommandType &type)
      : CommandImplementation(type)
   { }

   virtual ~BatchEvalCommand();
   bool Apply(const CommandContext &context) override;
};

#endif /* End of include guard: __BATCHEVALCOMMAND__ */
