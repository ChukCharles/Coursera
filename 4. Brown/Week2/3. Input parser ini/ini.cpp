#include "ini.h"

namespace Ini {
	Section& Document::AddSection(string name) {
		return sections[name];
	}

	const Section& Document::GetSection(const string& name) const {
		return sections.at(name);
	}

	size_t Document::SectionCount() const {
		return sections.size();
	}

	Document Load(istream& input) {
		Document doc;
		string line;
		Section* sec = new Section();
		while (getline(input, line)) {
			if (line.size() == 0) continue;

			if (line[0] == '[') {
				sec = &doc.AddSection(line.substr(1, line.size() - 2));
				continue;
			}
			sec->insert(ParseLine(line));
		}
		return doc;
	}

	pair<string, string> ParseLine(string line) {
		size_t pos = line.find("=");
		string key = line.substr(0, pos);
		string val = line.substr(pos + 1);
		return make_pair(key, val);
	}
}