ENTRY(reset_handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 2048K 
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 512K 
}

SECTIONS
{
    PROVIDE( _estack = ORIGIN(RAM) + LENGTH(RAM));

    .text :
    {
        _stext = .;               
        KEEP(*(.vector_table)) 
        *(.text*)              
        *(.rodata*)            
        _etext = .;            
    } > FLASH
    
    _sdata_lma = LOADADDR(.data); /* .init, .fini, .extab, .exidx sections are put after .text, so this will take the .data start address from FLASH (LMA) */
    
    .data : 
    {
        _sdata = .;                  
        *(.data*)           
        _edata = .;         
    } > RAM AT >FLASH   
    
    .bss :
    {
        _sbss = .;       
        *(.bss*)        
        *(COMMON)       
        _ebss = .;      
    } > RAM

    . = ALIGN(4);
    
    .heap :
    {
    	_sheap = .; /* for _sbrk */
    	*(.heap*)    
    	_eheap = _estack; /* heap till the end of SRAM */
    } > RAM
}
