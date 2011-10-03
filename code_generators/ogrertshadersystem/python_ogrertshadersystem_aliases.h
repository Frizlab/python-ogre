// typedef name nicename;
typedef std::vector<Ogre::RTShader::Function*, Ogre::STLAllocator<Ogre::RTShader::Function*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorRTShaderFunction;
typedef std::vector<Ogre::RTShader::FunctionAtom*, Ogre::STLAllocator<Ogre::RTShader::FunctionAtom*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorRTShaderFunctionAtom;
typedef std::vector<Ogre::RTShader::Operand, Ogre::STLAllocator<Ogre::RTShader::Operand, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorRTShaderOperand;
//typedef std::vector<Ogre::RTShader::ShaderGenerator::SGPass*, Ogre::STLAllocator<Ogre::RTShader::ShaderGenerator::SGPass*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> stdVectorRTShaderShaderGenerator;
typedef std::vector<Ogre::RTShader::SubRenderState*, Ogre::STLAllocator<Ogre::RTShader::SubRenderState*, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorRTShaderSubRenderState;
typedef std::vector<Ogre::SharedPtr<Ogre::RTShader::Parameter>, Ogre::STLAllocator<Ogre::SharedPtr<Ogre::RTShader::Parameter>, Ogre::CategorisedAllocPolicy<(Ogre::MemoryCategory)0> > > stdVectorRTShaderSharedPtr;

typedef std::map<Ogre::RTShader::Parameter*,Ogre::SharedPtr<Ogre::RTShader::Parameter>,std::less<Ogre::RTShader::Parameter*>,Ogre::STLAllocator<std::pair<Ogre::RTShader::Parameter* const, Ogre::SharedPtr<Ogre::RTShader::Parameter> >, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > stdMapRTShaderParameterParameter;
typedef std::map<Ogre::RTShader::Parameter*,std::vector<Ogre::RTShader::Operand*, Ogre::STLAllocator<Ogre::RTShader::Operand*, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > >,std::less<Ogre::RTShader::Parameter*>,Ogre::STLAllocator<std::pair<Ogre::RTShader::Parameter* const, std::vector<Ogre::RTShader::Operand*, Ogre::STLAllocator<Ogre::RTShader::Operand*, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > >, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > stdMapRTShaderParameterOperand;
//typedef std::vector<Ogre::RTShader::ProgramProcessor::MergeParameter,Ogre::STLAllocator<Ogre::RTShader::ProgramProcessor::MergeParameter, Ogre::CategorisedAllocPolicy<Ogre::MEMCATEGORY_GENERAL> > > stdVectoRTShaderProgramProcessorMergeParameter;
//typedef std::vector<Ogre::RTShader::ProgramProcessor::MergeParameter,Ogre::STLAllocator<Ogre::RTShader::ProgramProcessor::MergeParameter, Ogre::CategorisedAllocPolicy<MEMCATEGORY_GENERAL> > >

