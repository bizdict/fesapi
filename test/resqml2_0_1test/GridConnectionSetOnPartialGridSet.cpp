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
#include "resqml2_0_1test/GridConnectionSetOnPartialGridSet.h"
#include "../config.h"
#include "../catch.hpp"
#include "resqml2_0_1/AbstractIjkGridRepresentation.h"
#include "resqml2_0_1/UnstructuredGridRepresentation.h"
#include "resqml2_0_1/ContinuousProperty.h"
#include "resqml2/GridConnectionSetRepresentation.h"

using namespace std;
using namespace COMMON_NS;
using namespace resqml2_0_1test;
using namespace RESQML2_NS;

const char* GridConnectionSetOnPartialGridSet::defaultUuid = "a6fa81a3-f703-4026-9b28-606311761235";
const char* GridConnectionSetOnPartialGridSet::defaultTitle = "Grid Connection Set multi grids";

GridConnectionSetOnPartialGridSet::GridConnectionSetOnPartialGridSet(const string & epcDocPath)
	: AbstractResqmlDataObjectTest(epcDocPath, defaultUuid, defaultTitle) {
}

GridConnectionSetOnPartialGridSet::GridConnectionSetOnPartialGridSet(COMMON_NS::EpcDocument * epcDoc, bool init)
	: AbstractResqmlDataObjectTest(epcDoc, defaultUuid, defaultTitle) {
		if (init)
			this->initEpcDoc();
		else
			this->readEpcDoc();
}

void GridConnectionSetOnPartialGridSet::initEpcDocHandler() {
	// getting the hdf proxy
	AbstractHdfProxy* hdfProxy = this->epcDoc->getHdfProxySet()[0];

	// Unstructured grid
	RESQML2_0_1_NS::UnstructuredGridRepresentation* partialGrid = this->epcDoc->createPartialUnstructuredGridRepresentation(uuidPartialUnstructuredGridRepresentation, titlePartialUnstructuredGridRepresentation);
	REQUIRE( partialGrid != nullptr );
	RESQML2_0_1_NS::ContinuousProperty* continuousProperty = this->epcDoc->createContinuousProperty(partialGrid, "62d7e07d-5e17-4e42-b4b2-a4c26204cba2", "Continuous prop on partial unstructured grid", 1,
		gsoap_resqml2_0_1::resqml2__IndexableElements__cells, 
		gsoap_resqml2_0_1::resqml2__ResqmlUom__m, 
		gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__length);
	double continuousProp1Values[6] = { 0, 1, 2, 3, 4, 5 };
	continuousProperty->pushBackDoubleHdf5Array1dOfValues(continuousProp1Values, 6, hdfProxy);

	// IJK grid
	RESQML2_0_1_NS::AbstractIjkGridRepresentation* partialIjkGrid = epcDoc->createPartialIjkGridRepresentation("b0ec8bf4-9b93-428b-a814-87c38887f6d0", "PartialIjk Grid");
	REQUIRE(partialIjkGrid != nullptr);
	RESQML2_0_1_NS::ContinuousProperty* continuousPropertyOnIjk = epcDoc->createContinuousProperty(partialIjkGrid, "b20299b9-6881-4b91-ae2f-a87213437dce", "Continuous prop on partial ijk grid", 1,
		gsoap_resqml2_0_1::resqml2__IndexableElements__cells,
		gsoap_resqml2_0_1::resqml2__ResqmlUom__m,
		gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__length);
	double continuousPropOnIjkValues[6] = { 0, 1, 2, 3, 4, 5 };
	continuousPropertyOnIjk->pushBackDoubleHdf5Array1dOfValues(continuousPropOnIjkValues, 6, hdfProxy);

	// Truncated IJK grid
	RESQML2_0_1_NS::AbstractIjkGridRepresentation* partialTruncIjkGrid = epcDoc->createPartialTruncatedIjkGridRepresentation("def167fb-89b2-45bc-92ff-01d228142350", "PartialIjk Grid");
	REQUIRE(partialIjkGrid != nullptr);
	RESQML2_0_1_NS::ContinuousProperty* continuousPropertyOnTruncIjk = epcDoc->createContinuousProperty(partialTruncIjkGrid, "4caa8e9a-00b3-40c2-9460-72cb8790393a", "Continuous prop on partial truncated ijk grid", 1,
		gsoap_resqml2_0_1::resqml2__IndexableElements__cells,
		gsoap_resqml2_0_1::resqml2__ResqmlUom__m,
		gsoap_resqml2_0_1::resqml2__ResqmlPropertyKind__length);
	double continuousPropOnTruncIjkValues[6] = { 0, 1, 2, 3, 4, 5 };
	continuousPropertyOnTruncIjk->pushBackDoubleHdf5Array1dOfValues(continuousPropOnTruncIjkValues, 6, hdfProxy);

	// Grid Connection Set on one grid
	RESQML2_NS::GridConnectionSetRepresentation* gcs = this->epcDoc->createGridConnectionSetRepresentation("c0214c71-eed8-4ea2-9de4-f7508caeb3c6", "Single grid gcs");
	gcs->pushBackSupportingGridRepresentation(partialGrid);
	ULONG64 cellConn[2] = {
		1, 2
	};
	gcs->setCellIndexPairs(1, cellConn, -1, hdfProxy);

	// Grid Connection Set on several grid
	RESQML2_NS::GridConnectionSetRepresentation* gcsMultiGrids = this->epcDoc->createGridConnectionSetRepresentation(defaultUuid, defaultTitle);
	gcsMultiGrids->pushBackSupportingGridRepresentation(partialGrid);
	gcsMultiGrids->pushBackSupportingGridRepresentation(partialIjkGrid);
	gcsMultiGrids->pushBackSupportingGridRepresentation(partialTruncIjkGrid);
	ULONG64 cellConnMultiGrids[6] = {
		1, 2, 1, 2, 1, 2
	};
	ULONG64 multiGridIndices[6] = {
		0, 0, 1, 1, 1, 2
	};
	gcsMultiGrids->setCellIndexPairs(3, cellConnMultiGrids, -1, hdfProxy, multiGridIndices);

}

void GridConnectionSetOnPartialGridSet::readEpcDocHandler() {
	// getting the ContinuousProperty
	RESQML2_NS::GridConnectionSetRepresentation* gcsSingleGrid = epcDoc->getResqmlAbstractObjectByUuid<RESQML2_NS::GridConnectionSetRepresentation>("c0214c71-eed8-4ea2-9de4-f7508caeb3c6");
	RESQML2_NS::GridConnectionSetRepresentation* gcsMultiGrids = epcDoc->getResqmlAbstractObjectByUuid<RESQML2_NS::GridConnectionSetRepresentation>(uuid);

	// checking that the supporting representation is partial
	REQUIRE(!gcsSingleGrid->isBasedOnMultiGrids());
	REQUIRE(gcsSingleGrid->getSupportingGridRepresentationCount() == 1);
	REQUIRE(gcsSingleGrid->getSupportingGridRepresentation(0)->isPartial());
	REQUIRE(gcsSingleGrid->getCellIndexPairCount() == 1);
	ULONG64* cellIndexPair = new ULONG64[2];
	gcsSingleGrid->getCellIndexPairs(cellIndexPair);
	REQUIRE(cellIndexPair[0] == 1);
	REQUIRE(cellIndexPair[1] == 2);
	delete[] cellIndexPair;

	REQUIRE(gcsMultiGrids->isBasedOnMultiGrids());
	REQUIRE(gcsMultiGrids->getSupportingGridRepresentationCount() == 3);
	REQUIRE(gcsMultiGrids->getSupportingGridRepresentation(0)->isPartial());
	REQUIRE(gcsMultiGrids->getSupportingGridRepresentation(1)->isPartial());
	REQUIRE(gcsMultiGrids->getSupportingGridRepresentation(2)->isPartial());
	REQUIRE(gcsMultiGrids->getCellIndexPairCount() == 3);
	cellIndexPair = new ULONG64[6];
	gcsMultiGrids->getCellIndexPairs(cellIndexPair);
	REQUIRE(cellIndexPair[0] == 1);
	REQUIRE(cellIndexPair[1] == 2);
	REQUIRE(cellIndexPair[2] == 1);
	REQUIRE(cellIndexPair[3] == 2);
	REQUIRE(cellIndexPair[4] == 1);
	REQUIRE(cellIndexPair[5] == 2);
	delete[] cellIndexPair;
	ULONG64* gridIndexPair = new ULONG64[6];
	gcsMultiGrids->getGridIndexPairs(gridIndexPair);
	REQUIRE(gridIndexPair[0] == 0);
	REQUIRE(gridIndexPair[1] == 0);
	REQUIRE(gridIndexPair[2] == 1);
	REQUIRE(gridIndexPair[3] == 1);
	REQUIRE(gridIndexPair[4] == 1);
	REQUIRE(gridIndexPair[5] == 2);
	delete[] gridIndexPair;
}

