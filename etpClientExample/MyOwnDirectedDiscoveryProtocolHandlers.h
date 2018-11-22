/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#pragma once

#include "etp/ProtocolHandlers/DirectedDiscoveryHandlers.h"

#include "MyOwnEtpClientSession.h"

#include "common/AbstractObject.h"

class MyOwnDirectedDiscoveryProtocolHandlers : public ETP_NS::DirectedDiscoveryHandlers
{
public:
	MyOwnDirectedDiscoveryProtocolHandlers(MyOwnEtpClientSession* mySession): ETP_NS::DirectedDiscoveryHandlers(mySession) {}
	~MyOwnDirectedDiscoveryProtocolHandlers() {}

	std::vector<int64_t> getObjectWhenDiscovered; // all message id in this vector will result in response where the objects are going to be get in addition to be discovered

	void on_GetResourcesResponse(const Energistics::Etp::v12::Protocol::DirectedDiscovery::GetResourcesResponse & grr, int64_t correlationId);
};