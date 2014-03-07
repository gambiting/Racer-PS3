########################################################
# PS3 PPU Makefile generated for GNU Make by ProDG VSI #
########################################################

.SILENT:

unexport MAKEFLAGS
unexport MFLAGS

OUTDIR = PS3_SNC_PPU_Debug
INTDIR = PS3_SNC_PPU_Debug

LIBS=\
	libsn.a\
	libm.a\
	libio_stub.a\
	libfs_stub.a\
	libcgc.a\
	-lcgb\
	-lgcm_cmd\
	-lgcm_sys_stub\
	-lio_stub\
	-lsysutil_stub\

WLIBS=\

AS=ppu-lv2-gcc
AS_FLAGS=\
	-DSN_TARGET_PS3\
	-D_DEBUG\
	-D__SNC__\
	-D__CELL_ASSERT__\
	-I.\
	-c\
	-D__GCC__\
	-g\

CC=ps3ppusnc
CC_FLAGS=\
	-DSN_TARGET_PS3\
	-D_DEBUG\
	-D__SNC__\
	-D__CELL_ASSERT__\
	-I.\
	-c\
	-Xmserrors\
	-Xdepmode=1\
	-MMD\
	-g\
	-Xc-=rtti\

CXX=ps3ppusnc
CXX_FLAGS=\
	-DSN_TARGET_PS3\
	-D_DEBUG\
	-D__SNC__\
	-D__CELL_ASSERT__\
	-I.\
	-c\
	-Xmserrors\
	-Xdepmode=1\
	-MMD\
	-g\
	-Xc-=rtti\

LD=ps3ppuld
LD_FLAGS=\
	-oformat=fself\

OBJS=\
	"PS3_SNC_PPU_Debug/Camera.obj"\
	"PS3_SNC_PPU_Debug/GCMRenderer.obj"\
	"PS3_SNC_PPU_Debug/HeightMap.obj"\
	"PS3_SNC_PPU_Debug/Input.obj"\
	"PS3_SNC_PPU_Debug/main.obj"\
	"PS3_SNC_PPU_Debug/Mesh.obj"\
	"PS3_SNC_PPU_Debug/Renderer.obj"\
	"PS3_SNC_PPU_Debug/SceneNode.obj"\
	"PS3_SNC_PPU_Debug/Shader.obj"\
	"PS3_SNC_PPU_Debug/TextMesh.obj"\
	"PS3_SNC_PPU_Debug/Timer.obj"\

CST_OBJS=\

CST_OUTS=\
	fragment.fpo\
	fragment_asm.txt\
	vertex.vpo\

CUSTOM: $(CST_OUTS) $(CST_OBJS)

"fragment_asm.txt":
"fragment.fpo":
	cmd /c "PS3_SNC_PPU_Debug\fragment.cg.bat"

"vertex.vpo":
	cmd /c "PS3_SNC_PPU_Debug\vertex.cg.bat"

COMPILE: $(OBJS)

"PS3_SNC_PPU_Debug/Camera.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/Camera.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/Camera.cpp" -o $@

"PS3_SNC_PPU_Debug/GCMRenderer.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/GCMRenderer.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/GCMRenderer.cpp" -o $@

"PS3_SNC_PPU_Debug/HeightMap.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/HeightMap.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/HeightMap.cpp" -o $@

"PS3_SNC_PPU_Debug/Input.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/Input.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/Input.cpp" -o $@

"PS3_SNC_PPU_Debug/main.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/main.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/main.cpp" -o $@

"PS3_SNC_PPU_Debug/Mesh.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/Mesh.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/Mesh.cpp" -o $@

"PS3_SNC_PPU_Debug/Renderer.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/Renderer.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/Renderer.cpp" -o $@

"PS3_SNC_PPU_Debug/SceneNode.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/SceneNode.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/SceneNode.cpp" -o $@

"PS3_SNC_PPU_Debug/Shader.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/Shader.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/Shader.cpp" -o $@

"PS3_SNC_PPU_Debug/TextMesh.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/TextMesh.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/TextMesh.cpp" -o $@

"PS3_SNC_PPU_Debug/Timer.obj":
	@echo ProDG Compiling "d:/Team Project/Racer-PS3/GCM Framework/Timer.cpp"
	$(CXX) $(CXX_FLAGS) "d:/Team Project/Racer-PS3/GCM Framework/Timer.cpp" -o $@

LINK: "PS3_SNC_PPU_Debug/GCM Framework.ppu.self"

"PS3_SNC_PPU_Debug/GCM Framework.ppu.self" : $(CST_OBJS) $(OBJS)
	@echo ProDG Linking $@
	$(LD) $(LD_FLAGS) --start-group $(CST_OBJS) $(OBJS) $(LIBS) --end-group -o $@

