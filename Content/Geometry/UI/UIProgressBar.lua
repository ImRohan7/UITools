return
{
-- This is geometry info from UI HealthBar.

-- Followings are the screen dimensions
-- Width 100
-- Height 100
-- Therefore the attributes should not exceed than 100
-- else they will be out of screen
-- For position while creating object in Game consider center of screen as (0,0)
-- ie you can go -100 for left edge and +100 for right, -100 for bottom and +100 for top

-- The type of item
Type = "progress_bar",

-- The height and width for Bar
-- I recommend the standard(good looking) bar dimesnions which are 15x50
Dimensions =
{
	height = 10,
	width = 50,
},

-- main color ( the color for filled bar)
color = { r=0, g=1, b=0.1, a=1, },

-- background color (the color for unfilled are of the bar)
bg_color = { r=0.5, g=0, b=1.0, a=1, },

}
