#include "resiprocate/TransactionUser.hxx"
#include "resiprocate/MessageFilterRule.hxx"
#include "resiprocate/os/Logger.hxx"

#define RESIPROCATE_SUBSYSTEM resip::Subsystem::TRANSACTION

using namespace resip;

TransactionUser::TransactionUser() : mFifo(0, 0)
{
  // This creates a default message filter rule, which
  // handles all sip: and sips: requests.
  mRuleList.push_back(MessageFilterRule());
}

TransactionUser::TransactionUser(MessageFilterRuleList &mfrl) 
  : mFifo(0, 0), mRuleList(mfrl)
{
}


TransactionUser::~TransactionUser()
{
}

void 
TransactionUser::post(Message* msg)
{
  mFifo.add(msg, TimeLimitFifo<Message>::InternalElement);
}

void 
TransactionUser::postToTransactionUser(Message* msg, TimeLimitFifo<Message>::DepthUsage usage)
{
   mFifo.add(msg, usage);
   DebugLog (<< "TransactionUser::postToTransactionUser " << msg->brief() << " &=" << &mFifo << " size=" << mFifo.size());
}

unsigned int 
TransactionUser::size() const
{
   return mFifo.size();
}    

bool 
TransactionUser::wouldAccept(TimeLimitFifo<Message>::DepthUsage usage) const
{
   return mFifo.wouldAccept(usage);
}

bool
TransactionUser::isForMe(const SipMessage& msg) const
{
   // do this for each MessageFilterRule
   for (MessageFilterRuleList::const_iterator i = mRuleList.begin() ; 
        i != mRuleList.end() ; ++i)
   {
       if (i->matches(msg))
       {
          return true;
       }       
   }
   return false;
}

bool 
TransactionUser::isMyDomain(const Uri& uri) const
{
   for (DomainList::const_iterator it = mDomainList.begin(); 
        it != mDomainList.end(); it++)
   {
      if(uri.host() == it->host() && uri.port() == it->port())
      {
         return true;
      }
   }
   return false;
}

bool 
TransactionUser::isMyDomain(const Data& domain) const
{
   for (DomainList::const_iterator it = mDomainList.begin(); 
        it != mDomainList.end(); it++)
   {
      if(domain == it->host())
      {
         return true;
      }
   }
   return false;
}

void TransactionUser::addDomain(const Data& domain)
{
   Uri u;
   u.host() = domain;
   mDomainList.push_back(u);
}

void TransactionUser::addDomain(const Data& domain, int port)
{
   Uri u;
   u.host() = domain;
   u.port() = port;
   mDomainList.push_back(u);
} 

void
TransactionUser::setMessageFilterRuleList(MessageFilterRuleList &rules)
{
   mRuleList = rules;
}


/* ====================================================================
 * The Vovida Software License, Version 1.0 
 * 
 * Copyright (c) 2000 Vovida Networks, Inc.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 
 * 3. The names "VOCAL", "Vovida Open Communication Application Library",
 *    and "Vovida Open Communication Application Library (VOCAL)" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact vocal@vovida.org.
 *
 * 4. Products derived from this software may not be called "VOCAL", nor
 *    may "VOCAL" appear in their name, without prior written
 *    permission of Vovida Networks, Inc.
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND
 * NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL VOVIDA
 * NETWORKS, INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT DAMAGES
 * IN EXCESS OF $1,000, NOR FOR ANY INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 * ====================================================================
 * 
 * This software consists of voluntary contributions made by Vovida
 * Networks, Inc. and many individuals on behalf of Vovida Networks,
 * Inc.  For more information on Vovida Networks, Inc., please see
 * <http://www.vovida.org/>.
 *
 */
