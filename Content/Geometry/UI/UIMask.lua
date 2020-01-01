return
{
-- This is geometry info from UI Mask.
-- Notes ----------------------
-- Followings are the screen dimensions
-- Width 100
-- Height 100
-- Therefore the attributes should not exceed than 100
-- else they will be out of screen
-- For position while creating object in Game consider center of screen as (0,0)
-- ie you can go -100 for left edge and +100 for right, -100 for bottom and +100 for top

-- The type of item
Type = "mask",

-- The height and width for Bar
-- I recommend the standard(good looking) bar dimesnions which are 15x50
Dimensions =
{
	height = 40,
	width = 100,
},

-- color for the panel
color = { r=0.4, g=0.4, b=0.5, a=1, },

}