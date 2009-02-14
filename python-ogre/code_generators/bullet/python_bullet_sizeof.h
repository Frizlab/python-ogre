// sizeof ( variable );
sizeof ( btAlignedObjectArray<btOptimizedBvhNode>	); //NodeArray;
sizeof ( btAlignedObjectArray<btQuantizedBvhNode>	); //QuantizedNodeArray;
sizeof (  btAlignedObjectArray<btBvhSubtreeInfo>	); //	BvhSubtreeInfoArray;
//sizeof ( btStackAlloc );
//sizeof ( Hull );
//sizeof ( btSimulationIslandManager );
sizeof ( PHY_ScalarType );
sizeof ( btTypedConstraintType );
sizeof ( btAxisSweep3Internal<unsigned int> );
sizeof ( btAxisSweep3Internal<unsigned short> );
sizeof ( btDynamicsWorldType );
sizeof (  btAlignedObjectArray<btWheelInfo> );
sizeof ( btBroadphaseInterface );
sizeof ( btCollisionObject );
sizeof ( btRaycastVehicle );

#ifdef WIN32 
    sizeof ( Win32ThreadSupport::btSpuStatus );
#endif    