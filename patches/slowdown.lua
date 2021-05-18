Script.properties({
  name = 'Slowdown Fix',
  description = 'Removes the slowdown during effects',
  author = 'Archaemic',
  version = '1.0.0'
})

Patch.write8(0x08a0ef80, 0x02)
Patch.write8(0x08a0ef90, 0x03)
Patch.write8(0x08a0ef9c, 0x02)
