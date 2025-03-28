#include "cli.h"

int main()
{
	lxr::cli::init_cli();

	bool is_running = true;

	while(is_running) {
		std::string line = lxr::cli::readcli();
		if (line.empty()) {
			continue;
		}

		std::stringstream ss(line);
		std::string buf;

		// 分号分隔出多条命令
		while (std::getline(ss, buf, ';')) {
			if (buf.empty()) {
				continue;
			}
			std::printf("[INFO] input:%s\n", buf.c_str());
			if (buf == "exit") {
				is_running = false;
				break;
			}
		}
	}

	return 0;
}
