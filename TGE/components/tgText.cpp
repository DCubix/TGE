#include "tgText.h"

void tgText::colorize(tgVector4 const& color, int range_start, int range_len) {
	if (range_len == TEXT_LEN) {
		range_len = m_text.size();
	}

	// Checks if range is already added
	bool has_range = false;
	tgTextRange tr;
	for (tgTextRange r : m_rangeProperties) {
		if (r.start == range_start && r.length == range_len) {
			tr = r;
			has_range = true;
			break;
		}
	}
	tr.color = color;
	tr.start = range_start;
	tr.length = range_len;
	if (!has_range) {
		m_rangeProperties.push_back(tr);
	}
}
