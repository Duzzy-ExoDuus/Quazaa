/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#ifndef D_METALINK_PARSER_STATE_MACHINE_H
#define D_METALINK_PARSER_STATE_MACHINE_H

#include "common.h"
#include <string>
#include <vector>
#include <stack>

#include "SharedHandle.h"
#include "MetalinkParserController.h"
#include "MetalinkParserState.h"

namespace aria2 {

class Metalinker;

class MetalinkParserStateMachine {
private:
  SharedHandle<MetalinkParserController> ctrl_;

  std::stack<MetalinkParserState*> stateStack_;

  // Error messages encountered while parsing document.
  std::vector<std::string> errors_;

  static MetalinkParserState* initialState_;
  static MetalinkParserState* skipTagState_;
  
  // Metalink3
  static MetalinkParserState* metalinkState_;
  static MetalinkParserState* filesState_; // Metalink3Spec
  static MetalinkParserState* fileState_;
  static MetalinkParserState* sizeState_;
  static MetalinkParserState* versionState_;
  static MetalinkParserState* languageState_;
  static MetalinkParserState* osState_;
  static MetalinkParserState* verificationState_; // Metalink3Spec
  static MetalinkParserState* hashState_;
  static MetalinkParserState* piecesState_; // Metalink3Spec
  static MetalinkParserState* pieceHashState_; // Metalink3Spec
  static MetalinkParserState* signatureState_;
  static MetalinkParserState* resourcesState_; // Metalink3Spec
  static MetalinkParserState* urlState_;

  // Metalink4
  static MetalinkParserState* metalinkStateV4_;
  static MetalinkParserState* fileStateV4_;
  static MetalinkParserState* sizeStateV4_;
  static MetalinkParserState* versionStateV4_;
  static MetalinkParserState* languageStateV4_;
  static MetalinkParserState* osStateV4_;
  static MetalinkParserState* hashStateV4_;
  static MetalinkParserState* piecesStateV4_; // Metalink4Spec
  static MetalinkParserState* pieceHashStateV4_; // Metalink4Spec
  static MetalinkParserState* signatureStateV4_;
  static MetalinkParserState* urlStateV4_;
  static MetalinkParserState* metaurlStateV4_;
public:
  MetalinkParserStateMachine();

  ~MetalinkParserStateMachine();

  void setSkipTagState();

  void setMetalinkState();

  void setFilesState(); // Metalink3Spec

  void setFileState();

  void setSizeState();

  void setVersionState();

  void setLanguageState();
  
  void setOSState();

  void setVerificationState(); // Metalink3Spec

  void setHashState();

  void setPiecesState(); // Metalink3Spec

  void setPieceHashState(); // Metalink3Spec

  void setSignatureState();

  void setResourcesState(); // Metalink3Spec

  void setURLState();

  // Metalink4
  void setMetalinkStateV4();
  void setFileStateV4();
  void setSizeStateV4();
  void setVersionStateV4();
  void setLanguageStateV4();
  void setOSStateV4();
  void setHashStateV4();
  void setPiecesStateV4(); // Metalink4Spec
  void setPieceHashStateV4(); // Metalink4Spec
  void setSignatureStateV4();
  void setURLStateV4();
  void setMetaurlStateV4();

  bool finished() const;

  void beginElement
  (const std::string& localname,
   const std::string& prefix,
   const std::string& nsUri,
   const std::vector<XmlAttr>& attrs);
  
  void endElement
  (const std::string& localname,
   const std::string& prefix,
   const std::string& nsUri,
   const std::string& characters);

  void newEntryTransaction();

  void setFileNameOfEntry(const std::string& filename);

  void setFileLengthOfEntry(uint64_t length);

  void setVersionOfEntry(const std::string& version);

  void setLanguageOfEntry(const std::string& language);

  void setOSOfEntry(const std::string& os);

  void setMaxConnectionsOfEntry(int maxConnections); // Metalink3Spec

  void commitEntryTransaction();

  void cancelEntryTransaction();

  void newResourceTransaction();

  void setURLOfResource(const std::string& url);

  void setTypeOfResource(const std::string& type);

  void setLocationOfResource(const std::string& location);

  void setPriorityOfResource(int priority);

  void setMaxConnectionsOfResource(int maxConnections); // Metalink3Spec

  void commitResourceTransaction();

  void cancelResourceTransaction();

  void newChecksumTransaction();

  void setTypeOfChecksum(const std::string& type);

  void setHashOfChecksum(const std::string& md);

  void commitChecksumTransaction();

  void cancelChecksumTransaction();

  void newChunkChecksumTransactionV4(); // Metalink4Spec

  void setLengthOfChunkChecksumV4(size_t length); // Metalink4Spec

  void setTypeOfChunkChecksumV4(const std::string& type); // Metalink4Spec

  void addHashOfChunkChecksumV4(const std::string& md); // Metalink4Spec

  void commitChunkChecksumTransactionV4(); // Metalink4Spec

  void cancelChunkChecksumTransactionV4(); // Metalink4Spec

  void newChunkChecksumTransaction(); // Metalink3Spec

  void setLengthOfChunkChecksum(size_t length); // Metalink3Spec

  void setTypeOfChunkChecksum(const std::string& type); // Metalink3Spec

  void createNewHashOfChunkChecksum(size_t order); // Metalink3Spec

  void setMessageDigestOfChunkChecksum(const std::string& md); // Metalink3Spec

  void addHashOfChunkChecksum(); // Metalink3Spec

  void commitChunkChecksumTransaction(); // Metalink3Spec

  void cancelChunkChecksumTransaction(); // Metalink3Spec

  void newSignatureTransaction();

  void setTypeOfSignature(const std::string& type);

  void setFileOfSignature(const std::string& file);

  void setBodyOfSignature(const std::string& body);

  void commitSignatureTransaction();

  void cancelSignatureTransaction();

  void newMetaurlTransaction();

  void setURLOfMetaurl(const std::string& url);

  void setMediatypeOfMetaurl(const std::string& mediatype);

  void setPriorityOfMetaurl(int priority);

  void setNameOfMetaurl(const std::string& name);

  void commitMetaurlTransaction();

  void cancelMetaurlTransaction();

  bool needsCharactersBuffering() const;

  // Only stores first 10 errors.
  void logError(const std::string& log);

  const std::vector<std::string>& getErrors() const
  {
    return errors_;
  }

  std::string getErrorString() const;

  const SharedHandle<Metalinker>& getResult() const
  {
    return ctrl_->getResult();
  }

  void setBaseUri(const std::string& uri);
};

} //  namespace aria2

#endif // D_METALINK_PARSER_STATE_MACHINE_H
