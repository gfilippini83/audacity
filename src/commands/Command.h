/**********************************************************************

   Audacity - A Digital Audio Editor
   Copyright 1999-2009 Audacity Team
   File License: wxWidgets

   Dan Horgan

******************************************************************//**

\file Command.h
\brief Contains declaration of Command base class.


*//*******************************************************************/

#ifndef __COMMAND__
#define __COMMAND__

#include <wx/app.h>
#include "../MemoryX.h"

#include "CommandMisc.h"
#include "CommandSignature.h"
#include "CommandTargets.h"
#include "../commands/AudacityCommand.h"

class AudacityApp;
class CommandContext;
class CommandOutputTarget;

// Abstract base class for command interface.  
class OldStyleCommand /* not final */
{
public:
   OldStyleCommand() {};
   virtual ~OldStyleCommand() { }
   virtual wxString GetName() = 0;
   virtual CommandSignature &GetSignature() = 0;
   virtual bool SetParameter(const wxString &paramName, const wxVariant &paramValue);
   virtual bool Apply()=0;
   virtual bool Apply(const CommandContext &context) = 0;
};

using OldStyleCommandPointer = std::shared_ptr<OldStyleCommand>;

/// Command which wraps another command
/// It ISA command and HAS a command.
class DecoratedCommand /* not final */ : public OldStyleCommand
{
protected:
   OldStyleCommandPointer mCommand;
public:
   DecoratedCommand(const OldStyleCommandPointer &cmd)
      : mCommand(cmd)
   {
      wxASSERT(cmd != NULL);
   }
   virtual ~DecoratedCommand();
   wxString GetName() override;
   CommandSignature &GetSignature() override;
   bool SetParameter(const wxString &paramName, const wxVariant &paramValue) override;
};

// Decorator command that performs the given command and then outputs a status
// message according to the result
class ApplyAndSendResponse final : public DecoratedCommand
{
public:
   ApplyAndSendResponse(const OldStyleCommandPointer &cmd, std::unique_ptr<CommandOutputTarget> &target);
   bool Apply() override;
   bool Apply(const CommandContext &context) override;// Error to use this.
   std::unique_ptr<CommandContext> mCtx;

};

class CommandImplementation /* not final */ : public OldStyleCommand
{
private:
   OldStyleCommandType &mType;
   ParamValueMap mParams;
   ParamBoolMap mSetParams;

   /// Using the command signature, looks up a possible parameter value and
   /// checks whether it passes the validator.
   bool Valid(const wxString &paramName, const wxVariant &paramValue);

protected:
   // Convenience methods for allowing subclasses to access parameters
   void TypeCheck(const wxString &typeName,
                  const wxString &paramName,
                  const wxVariant &param);
   void CheckParam(const wxString &paramName);
   bool HasParam( const wxString &paramName);
   bool GetBool(const wxString &paramName);
   long GetLong(const wxString &paramName);
   double GetDouble(const wxString &paramName);
   wxString GetString(const wxString &paramName);

public:
   /// Constructor should not be called directly; only by a factory which
   /// ensures name and params are set appropriately for the command.
   CommandImplementation(OldStyleCommandType &type);

   virtual ~CommandImplementation();

   /// An instance method for getting the command name (for consistency)
   wxString GetName();

   /// Get the signature of the command
   CommandSignature &GetSignature();

   /// Attempt to one of the command's parameters to a particular value.
   /// (Note: wxVariant is reference counted)
   bool SetParameter(const wxString &paramName, const wxVariant &paramValue);

   // Subclasses should override the following:
   // =========================================

   /// Actually carry out the command. Return true if successful and false
   /// otherwise.
   bool Apply() { return false;};// No longer supported.
   bool Apply(const CommandContext &context) override;
};

#endif /* End of include guard: __COMMAND__ */
