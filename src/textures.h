#ifndef TEXTURES_H
#define TEXTURES_H

#ifdef __cplusplus
extern "C"{
#if 0
}
#endif
#endif

struct wall_dat{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[1024 * 1024 * 4 + 1];
};
extern struct wall_dat wall_bytedata;

struct agent_dat{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[1024 * 1024 * 4 + 1];
};
extern struct agent_dat agent_bytedata;

struct energy_cell_dat{
  unsigned int 	 width;
  unsigned int 	 height;
  unsigned int 	 bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */ 
  unsigned char	 pixel_data[1024 * 1024 * 4 + 1];
};
extern struct energy_cell_dat energy_cell_bytedata;

#ifdef __cplusplus
}
#endif
#endif
