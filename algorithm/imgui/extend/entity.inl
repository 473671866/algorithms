#ifndef COLORS_INL
#define COLORS_INL

#define Red imgui::ColorConvertU32ToFloat4(IM_COL32(255, 0, 0, 1))
#define Green imgui::ColorConvertU32ToFloat4(IM_COL32(0, 128, 0, 1))
#define Yellow imgui::ColorConvertU32ToFloat4(IM_COL32(255, 255, 0, 1))
#define DimGray imgui::ColorConvertU32ToFloat4(IM_COL32(105, 105, 105, 1))
#define DarkGray imgui::ColorConvertU32ToFloat4(IM_COL32(169, 169, 169, 1))
#define DarkRed imgui::ColorConvertU32ToFloat4(IM_COL32(139, 0, 0, 1))

enum class clicked_flags {
	left = 1,
	right = 2,
	dbclick = 4
};

enum class staples :int {
	none,		//DarkGray
	watched,	//DimGray
	startend,	//Green
	endend,		//Yellow
	deadend,	//Red
};

struct pointer {
	int x;						//x坐标
	int y;						//y坐标
	int	value;					//权值

	bool operator<(const pointer& other) const {
		return value < other.value;
	}

	bool operator>(const pointer& other) const {
		return value > other.value;
	}
};

struct terrain {
	ImVec4 color;
	staples staple;
};

constexpr int counter = 19;

#endif // !COLORS_INL