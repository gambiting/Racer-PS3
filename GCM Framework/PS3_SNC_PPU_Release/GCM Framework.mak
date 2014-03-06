########################################################
# PS3 PPU Makefile generated for GNU Make by ProDG VSI #
########################################################

.SILENT:

unexport MAKEFLAGS
unexport MFLAGS

OUTDIR = PS3_SNC_PPU_Release
INTDIR = PS3_SNC_PPU_Release

LIBS=\
	libsn.a\
	libm.a\
	libio_stub.a\
	libfs_stub.a\
	libcgc.a\
	-lgcm_cmd\
	-lgcm_sys_stub\
	-lio_stub\
	-lsysutil_stub\

WLIBS=\

AS=ppu-lv2-gcc
AS_FLAGS=\
	-DSN_TARGET_PS3\
	-DNDEBUG\
	-D__SNC__\
	-I.\
	-c\
	-D__GCC__\

CC=ps3ppusnc
CC_FLAGS=\
	-DSN_TARGET_PS3\
	-DNDEBUG\
	-D__SNC__\
	-I.\
	-c\
	-Xmserrors\
	-Xdepmode=1\
	-MMD\
	-O2\
	-Xc-=rtti\

CXX=ps3ppusnc
CXX_FLAGS=\
	-DSN_TARGET_PS3\
	-DNDEBUG\
	-D__SNC__\
	-I.\
	-c\
	-Xmserrors\
	-Xdepmode=1\
	-MMD\
	-O2\
	-Xc-=rtti\

LD=ps3ppuld
LD_FLAGS=\
	-oformat=fself\

OBJS=\
	"PS3_SNC_PPU_Release/GCMRenderer.obj"\
	"PS3_SNC_PPU_Release/Input.obj"\
	"PS3_SNC_PPU_Release/main.obj"\
	"PS3_SNC_PPU_Release/Mesh.obj"\
	"PS3_SNC_PPU_Release/Renderer.obj"\
	"PS3_SNC_PPU_Release/Shader.obj"\
	"PS3_SNC_PPU_Release/Texture.obj"\
	"PS3_SNC_PPU_Release/SceneNode.obj"\
	"PS3_SNC_PPU_Release/CameraNode.obj"\

CST_OBJS=\

CST_OUTS=\

CUSTOM: $(CST_OUTS) $(CST_OBJS)

COMPILE: $(OBJS)

"PS3_SNC_PPU_Release/GCMRenderer.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/GCMRenderer.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/GCMRenderer.cpp" -o $@

"PS3_SNC_PPU_Release/Input.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/Input.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/Input.cpp" -o $@

"PS3_SNC_PPU_Release/main.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/main.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/main.cpp" -o $@

"PS3_SNC_PPU_Release/Mesh.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/Mesh.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/Mesh.cpp" -o $@

"PS3_SNC_PPU_Release/Renderer.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/Renderer.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/Renderer.cpp" -o $@

"PS3_SNC_PPU_Release/Shader.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/Shader.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/Shader.cpp" -o $@

"PS3_SNC_PPU_Release/Texture.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/Texture.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/Texture.cpp" -o $@

"PS3_SNC_PPU_Release/SceneNode.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/SceneNode.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/SceneNode.cpp" -o $@

"PS3_SNC_PPU_Release/CameraNode.obj":
	@echo ProDG Compiling "d:/PS3 Tutorial Source/GCM Framework/CameraNode.cpp"
	$(CXX) $(CXX_FLAGS) "d:/PS3 Tutorial Source/GCM Framework/CameraNode.cpp" -o $@

LINK: "PS3_SNC_PPU_Release/GCM Framework.ppu.self"

"PS3_SNC_PPU_Release/GCM Framework.ppu.self" : $(CST_OBJS) $(OBJS)
	@echo ProDG Linking $@
	$(LD) $(LD_FLAGS) --start-group $(CST_OBJS) $(OBJS) $(LIBS) --end-group -o $@

