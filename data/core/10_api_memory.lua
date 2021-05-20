Script.properties({
  name = 'CORE: API - Memory',
  description = 'TacticsCrafter\'s Memory API',
  author = 'Nax',
  version = '1.0.0'
})

local function inject_extra_memory_loader()
  local filepath_addr = 0x08a73344
  local injector_addr = 0x08a73390

  Patch.cstring(filepath_addr, "disc0:/PSP_GAME/SYSDIR/UPDATE/DATA.BIN")
  ASM.patch(injector_addr, [[
    addiu sp, -0x10
    sw ra, 0xc(sp)
    sw s0, 0x8(sp)
    sw a1, 0x4(sp)
    sw a0, 0x0(sp)

    la a0, ]] .. filepath_addr .. [[
    la a1, 1
    jal sceIoOpen
     la a2, 0

    move a0, v0
    move s0, v0
    la   a1, 0x097e9200
    lui  a2, 0x08a7
    jal sceIoRead
     lw   a2, 0x3560(a2)

    jal sceIoClose
     move a0, s0

    lw a0, 0x0(sp)
    lw a1, 0x4(sp)
    lw s0, 0x8(sp)
    lw ra, 0xc(sp)

    j 0x089efcc0
     addiu sp, 0x10
  ]])

  ASM.patch(0x089efed4, "lui a1, 0x08a7")
  ASM.patch(0x089efee0, "ori a1, 0x3390")
end

local alloc = Memory.alloc
Memory.alloc = function(size)
  if size > 0 then
    inject_extra_memory_loader()
    Memory.alloc = alloc
  end
  return alloc(size)
end
