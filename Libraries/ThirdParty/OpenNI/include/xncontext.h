/*****************************************************************************
*                                                                            *
*  OpenNI 1.0 Alpha                                                          *
*  Copyright (C) 2010 PrimeSense Ltd.                                        *
*                                                                            *
*  This file is part of OpenNI.                                              *
*                                                                            *
*  OpenNI is free software: you can redistribute it and/or modify            *
*  it under the terms of the GNU Lesser General Public License as published  *
*  by the Free Software Foundation, either version 3 of the License, or      *
*  (at your option) any later version.                                       *
*                                                                            *
*  OpenNI is distributed in the hope that it will be useful,                 *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
*  GNU Lesser General Public License for more details.                       *
*                                                                            *
*  You should have received a copy of the GNU Lesser General Public License  *
*  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.            *
*                                                                            *
*****************************************************************************/




#ifndef __XN_CONTEXT_H__
#define __XN_CONTEXT_H__

/**
 * @ingroup cref
 * @defgroup context Context
 * This page details functions for managing an OpenNI context.
 * 
 * @section init Initializing and Deinitializing OpenNI Context
 *
 * Initialization must be performed before using any OpenNI functionality, and similarly you
 * must not call any OpenNI functions after deinitialization. 
 *
 * @section trees Enumerating and Creating Production Trees
 *
 * A production tree is composed of a production node, and optionally a list of other
 * production trees needed for this production node. Each production node has a type (one of
 * the types defined by OpenNI spec), a vendor name, and a specific name (unique for this type
 * and vendor), with a version.
 * For example, a User generator may need a Depth generator, which in turn might need a Device node.
 *
 * @subsection results Enumeration Results
 *
 * The results from an enumeration operation is a pointer to the first node of a linked list, representing
 * all the different possibilities to get data of the requested type. A single possibility contains
 * the provider description, an optional instance name (that can be used by the provider as proprietary
 * information, like a device S/N), and the bExists field, which tells if this node already exists in 
 * the context (and as such, doesn't need to be created) or not, and a list of needed nodes by this node.
 * 
 * @subsection enumex Enumerating Example
 *
 * For example, let's take a look at an application in need of depth maps:
 * @code
// Enumerate for production trees that can produce depth
XnNodeInfoList* pNodesList;
nRetVal = xnEnumerateProductionTrees(pNiteCtx, XN_NODE_TYPE_DEPTH, NULL, &pNodesList);
CHECK_RC(nRetVal, "Enumerate");

// choose a tree (for simplicity, take first one)
XnNodeInfoListIterator itChosen = xnNodeInfoListGetFirst(pNodesList);
XnNodeInfo* pChosen = xnNodeInfoListGetCurrent(itChosen);

// create first one
XnNodeHandle hDepth;
nRetVal = xnCreateProductionTree(pNiteCtx, pChosen, &hDepth);
CHECK_RC(nRetVal, "Create");

// free the list (it is no longer needed)
xnNodeInfoListFree(pNodesList);

// TODO: use handle for configuration and data extraction
 * @endcode
 *
 * @{
*/

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "XnTypes.h"
#include "XnQueries.h"
#include "XnPrdNodeInfoList.h"

//---------------------------------------------------------------------------
// Functions
//---------------------------------------------------------------------------

/** @name Initialization
 *  Functions for initializing and shutting down an OpenNI context.
 *  @{
 */

/**
 * @brief Initializes the OpenNI library. 
 *
 * This function must be called before calling any other OpenNI function (except for @ref xnInitFromXmlFile())
 *
 * @param	ppContext	[out]	Output location for context pointer.
 */
XN_C_API XnStatus xnInit(XnContext** ppContext);

/**
 * @brief Runs an XML script in the given context.
 *
 * @param	pContext	[in]		The context to be configured.
 * @param	strFileName	[in]		The name of the file containing the script. for a full description of the XML structure, see @ref xmlscripts.
 * @param	pErrors		[in/out]	Optional. If provided, will be filled with enumeration errors.
 */
XN_C_API XnStatus xnContextRunXmlScriptFromFile(XnContext* pContext, const XnChar* strFileName, XnEnumerationErrors* pErrors);

/**
 * @brief Runs an XML script in the given context.
 *
 * @param	pContext	[in]		The context.
 * @param	xmlScript	[in]		A string representation of the XML script. for a full description of the XML structure, see @ref xmlscripts.
 * @param	pErrors		[in/out]	Optional. If provided, will be filled with enumeration errors.
 */
XN_C_API XnStatus xnContextRunXmlScript(XnContext* pContext, const XnChar* xmlScript, XnEnumerationErrors* pErrors);

/**
 * @brief Initializes OpenNI context, and then configures it using the given file.
 *
 * @param	strFileName	[in]		The name of the file to read configuration from.
 * @param	ppContext	[out]		Output location for context pointer.
 * @param	pErrors		[in/out]	Optional. If provided, will be filled with enumeration errors.
 */
XN_C_API XnStatus xnInitFromXmlFile(const XnChar* strFileName, XnContext** ppContext, XnEnumerationErrors* pErrors);

/**
 * @brief Opens a recording file, adding all nodes in it to the context.
 *
 * @param	pContext	[in]	The context.
 * @param	strFileName	[in]	The file to open.
 */
XN_C_API XnStatus xnContextOpenFileRecording(XnContext* pContext, const XnChar* strFileName);

/**
 * @brief Shuts down the OpenNI library. No other OpenNI function can be called after calling this
 * function.
 *
 * @param	pContext	[in]	The context to be destroyed.
 */
XN_C_API void xnShutdown(XnContext* pContext);

// @}

/** @name Enumeration
 *  Functions for enumeration and creation of nodes.
 *  @{
 */

/** 
 * @brief Enumerates all available production trees for a specific node type. The trees populated in the
 * list should be freed by calling @ref xnNodeInfoListFree() once not needed.
 *
 * @param	pContext		[in]		OpenNI context.
 * @param	Type			[in]		The requested node type.
 * @param	pQuery			[in]		Optional. A query object that can be used to filter results.
 * @param	ppTreesList		[out]		A list of possible production trees.
 * @param	pErrors			[in/out]	Optional. If provided, will be filled with enumeration errors.
 */
XN_C_API XnStatus xnEnumerateProductionTrees(
	XnContext* pContext,
	XnProductionNodeType Type, 
	const XnNodeQuery* pQuery,
	XnNodeInfoList** ppTreesList,
	XnEnumerationErrors* pErrors
	);

/** 
 * @brief Creates a production node. If the tree specifies additional needed nodes, and those nodes
 * do not exist, they will be created too, and passed to this node as input.
 * 
 * @param	pContext		[in]		OpenNI context.
 * @param	pTree			[in]		A production tree to create.
 * @param	phNode			[out]		A handle to the newly created node.
 */
XN_C_API XnStatus xnCreateProductionTree(
	 XnContext* pContext,
	 XnNodeInfo* pTree,
	 XnNodeHandle* phNode
	 );

/**
 * @brief Enumerates for production trees for a specific node type, and creates the first found tree.
 * This function is a shortcut version for using @ref xnEnumerateProductionTrees(), iterating the list,
 * and then calling @ref xnCreateProductionTree().
 *
 * @param	pContext		[in]		OpenNI context.
 * @param	type			[in]		The requested node type.
 * @param	pQuery			[in]		Optional. A query object that can be used to filter results.
 * @param	phNode			[out]		A handle to the newly created node.
 * @param	pErrors			[in/out]	Optional. If provided, will be filled with enumeration errors.
 */
XN_C_API XnStatus xnCreateAnyProductionTree(
	XnContext* pContext,
	XnProductionNodeType type,
	XnNodeQuery* pQuery,
	XnNodeHandle* phNode,
	XnEnumerationErrors* pErrors
	);

/**
 * @brief Creates a production node which is only a mock. This node does not represent an actual node, but only
 * keeps a state and implements an interface above it.
 * Mock nodes are useful when simulating nodes for playing recordings, or for use in tests.
 * See also @ref xnCreateMockNodeBasedOn().
 *
 * @param	pContext		[in]		OpenNI context.
 * @param	type			[in]		The type of the mock to create.
 * @param	strName			[in]		Optional. The name of the node. If NULL is provided, a name will be automatically generated.
 * @param	phNode			[out]		A handle to the newly created node.
 */
XN_C_API XnStatus xnCreateMockNode(
	XnContext* pContext, 
	XnProductionNodeType type,
	const XnChar* strName,
	XnNodeHandle* phNode
	);

/**
 * @brief Creates a production node which is only a mock, base on the type and properties of another node. 
 * This node does not represent an actual node, but only keeps a state and implements an interface above it.
 * Mock nodes are useful when simulating nodes for playing recordings, or for use in tests.
 * See also @ref xnCreateMockNode().
 *
 * @param	pContext		[in]		OpenNI context.
 * @param	hOriginalNode	[in]		A handle to an existing node on which the mock node will be based on.
 * @param	strName			[in]		The name of the node. If set to NULL, a name will be generated based on the name of hOriginalNode.
 * @param	phMockNode		[out]		A handle to the newly created mock node.
 */
XN_C_API XnStatus xnCreateMockNodeBasedOn(
	XnContext* pContext,
	XnNodeHandle hOriginalNode,
	const XnChar* strName,
	XnNodeHandle* phMockNode
	);

/**
 * @brief References a production node, increasing its reference count by 1.
 *
 * @param	hNode	[in]	A handle to the node.
 */
XN_C_API XnStatus xnProductionNodeAddRef(XnNodeHandle hNode);

/**
 * References a production node, increasing its reference count by 1.
 * Note: this function is deprecated. Please use @ref xnProductionNodeAddRef() instead.
 *
 * @param	hNode	[in]	A handle to the node.
 */
XN_C_API XnStatus XN_API_DEPRECATED("Please use xnProductionNodeAddRef() instead.") xnRefProductionNode(XnNodeHandle hNode);

/**
 * @brief Unreference a production node, decreasing its reference count by 1. If the reference count reaches zero,
 * the node will be destroyed.
 *
 * @param	hNode	[in]	A handle to the node.
 */
XN_C_API void xnProductionNodeRelease(XnNodeHandle hNode);

/**
 * Unreference a production node, decreasing its reference count by 1. If the reference count reaches zero,
 * the node will be destroyed.
 * Note: this function is deprecated. Please use @ref xnProductionNodeAddRef() instead.
 *
 * @param	hNode	[in]	A handle to the node.
 */
XN_C_API void XN_API_DEPRECATED("Please use xnProductionNodeRelease() instead.") xnUnrefProductionNode(XnNodeHandle hNode);

/**
 * @brief Gets a list of all existing node in the context. Each node that was returned increases its ref count.
 * The list must be freed using @ref xnNodeInfoListFree().
 *
 * @param	pContext	[in]	OpenNI context.
 * @param	ppList		[out]	A linked list of current existing nodes
 */
XN_C_API XnStatus xnEnumerateExistingNodes(XnContext* pContext, XnNodeInfoList** ppList);

/**
 * @brief Gets a list of all existing node in the context. Each node that was returned increases its ref count.
 * The list must be freed using @ref xnNodeInfoListFree().
 *
 * @param	pContext	[in]	OpenNI context.
 * @param	type		[in]	Type to look for.
 * @param	ppList		[out]	A linked list of current existing nodes
 */
XN_C_API XnStatus xnEnumerateExistingNodesByType(XnContext* pContext, XnProductionNodeType type, XnNodeInfoList** ppList);

/**
 * @brief Returns the first found existing node of the specified type.
 *
 * @param	pContext	[in]	OpenNI context.
 * @param	type		[in]	Type to look for.
 * @param	phNode		[out]	A handle to the found node.
 */
XN_C_API XnStatus xnFindExistingNodeByType(
	XnContext* pContext,
	XnProductionNodeType type,
	XnNodeHandle* phNode
	);

/**
 * @brief Gets a handle to an existing production node instance using that instance name.
 *
 * @param	pContext		[in]	OpenNI context.
 * @param	strInstanceName	[in]	Name of the instance to get.
 * @param	phNode			[out]	A handle to that instance.
 *
 * @return XN_STATUS_BAD_NODE_NAME if node by name of strInstanceName doesn't exist.
 */
XN_C_API XnStatus xnGetNodeHandleByName(
	XnContext* pContext,
	const XnChar* strInstanceName,
	XnNodeHandle* phNode
	);

// @}

/** @name Multi-Node Handling
 *  Functions that affect all the nodes in the context.
 *  @{
 */

/**
 * @brief Updates all generators nodes in the context, waiting for all to have new data.
 *
 * @param	pContext		[in]	OpenNI context.
 */
XN_C_API XnStatus xnWaitAndUpdateAll(XnContext* pContext);

/**
 * @brief Updates all generators nodes in the context, waiting for a specific one to have new data.
 *
 * @param	pContext		[in]	OpenNI context.
 * @param	hNode			[in]	The node to wait for.
 */
XN_C_API XnStatus xnWaitOneUpdateAll(XnContext* pContext, XnNodeHandle hNode);

/**
 * @brief Updates all generators nodes in the context, once any of them have new data.
 *
 * @param	pContext		[in]	OpenNI context.
 */
XN_C_API XnStatus xnWaitAnyUpdateAll(XnContext* pContext);

/**
 * @brief Updates all generator nodes in the context, without any waiting. If a node has new data,
 * it will be updated.
 *
 * @param	pContext		[in]	OpenNI context.
 */
XN_C_API XnStatus xnWaitNoneUpdateAll(XnContext* pContext);

/**
 * @brief Make sure all generators are generating data.
 *
 * @param	pContext		[in]	OpenNI context.
 */
XN_C_API XnStatus xnStartGeneratingAll(XnContext* pContext);

/**
 * @brief Stop all generators from generating data.
 *
 * @param	pContext		[in]	OpenNI context.
 */
XN_C_API XnStatus xnStopGeneratingAll(XnContext* pContext);

/**
 * @brief Sets the global mirror flag. This will set all current existing nodes' mirror state, and also 
 * affect future created nodes. The default mirror flag is FALSE.
 *
 * @param	pContext		[in]	OpenNI context.
 * @param	bMirror			[in]	New Mirror state.
 */
XN_C_API XnStatus xnSetGlobalMirror(XnContext* pContext, XnBool bMirror);

/**
 * @brief Gets the global mirror flag.
 *
 * @param	pContext		[in]	OpenNI context.
 */
XN_C_API XnBool xnGetGlobalMirror(XnContext* pContext);

/**
 * @brief Gets the global error state of the context. If one of the nodes in the context is in error state,
 * that state will be returned. If more than one node is in error state, XN_STATUS_MULTIPLE_NODES_ERROR
 * is returned. An application can query each node error state by calling @ref xnGetNodeErrorState().
 *
 * @param	pContext		[in]	OpenNI context.
 */
XN_C_API XnStatus xnGetGlobalErrorState(XnContext* pContext);

/**
 * @brief Registers a callback function to global error state changes.
 *
 * @param	pContext	[in]	OpenNI context.
 * @param	handler		[in]	A pointer to a function that will be called when global error state changes.
 * @param	pCookie		[in]	A user cookie that will be passed to the callback function.
 * @param	phCallback	[out]	Optional. Will be filled with a handle to be passed to @ref xnUnregisterFromGlobalErrorStateChange().
 */
XN_C_API XnStatus xnRegisterToGlobalErrorStateChange
	(XnContext* pContext, XnErrorStateChangedHandler handler,
	void* pCookie, XnCallbackHandle* phCallback);

/**
 * @brief Unregisters a callback function which was registered using @ref xnRegisterToGlobalErrorStateChange().
 *
 * @param	pContext	 [in]	OpenNI context.
 * @param	hCallback	[in]	The handle to the callback returned from @ref xnRegisterToGlobalErrorStateChange().
 */
XN_C_API void xnUnregisterFromGlobalErrorStateChange
	(XnContext* pContext, XnCallbackHandle hCallback);

/// @}

/** @} */

#endif // __XN_CONTEXT_H__
