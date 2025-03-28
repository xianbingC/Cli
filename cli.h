#pragma once
#include <readline/readline.h>
#include <readline/history.h>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <cstring>


namespace lxr::cli {

// 默认提示符 
static const char* g_prompt = "cli% ";

// 用户单次输入的字符串(以'\'结尾的换行算单次输入)
static std::string g_input_buffer;

// 0表示单行输入，1表示多行输入(以'\'结尾)
static int g_cli_state = 0;

static std::map<std::string, std::vector<const char*>> g_cmd_map {
	{"list", {}},
	{"find", {"-name", "-depth", "-help"}},
	{"exit", {}}
};

static std::string delete_extra_space(const char* str)
{
	if (!str) {
		return "";
	}

	std::stringstream ss(str);
	std::string temp_buf;
	std::string res;

	while (std::getline(ss, temp_buf, ' ')) {
		if (!temp_buf.empty()) {
			res.append(temp_buf);
			res.push_back(' ');
		}
	}

	if (!res.empty()) {
		res.pop_back();
	}

	return res;
}

static void retrieve_match_cmd(std::vector<const char*>& matches, const char* text)
{
	std::string complete_input_buf = g_input_buffer;
	complete_input_buf.append(rl_line_buffer);

	// 获取最后一个正在输入的命令
	std::string last_cmd;
	size_t pos = complete_input_buf.rfind(';');
	if (pos == std::string::npos) {
		last_cmd = complete_input_buf;
	} else {
		last_cmd = complete_input_buf.substr(pos + 1);
	}

	// 截断正在输入的字符串
	pos = last_cmd.rfind(text);
	std::string last_prev_cmd = last_cmd.substr(0, pos);

	last_prev_cmd = delete_extra_space(last_prev_cmd.c_str());

	if (last_prev_cmd.empty()) {
		for (auto& item : g_cmd_map) {
			matches.push_back(item.first.c_str());
		}
		return ;
	}

	// 获取命令关键字 
	pos = last_prev_cmd.find(' ');
	std::string last_cmd_keyword = last_prev_cmd.substr(0, pos);

	auto iter = g_cmd_map.find(last_cmd_keyword);
	if (iter != g_cmd_map.end()) {
		matches = iter->second;
	}

}

static char* cmd_generator(const char* text, int state)
{
	static size_t idx = 0;
	static size_t len = 0;
	static std::vector<const char*> matches; 

	// 初始匹配状态
	if (0 == state) {
		idx = 0;
		len = strlen(text);
		retrieve_match_cmd(matches, text);
	}


	while (idx < matches.size()) {
		const char* match_text = matches[idx++];
		if (!strncmp(match_text, text, len)) {
			// 可选项必须输入'-'才提示，避免和文件提示冲突
			if (len == 0 && match_text[0] == '-') {
				continue;
			}
			return strdup(match_text);
		}
	}

	return NULL;
}

static char** cmd_completion(const char* text, int start, int end) 
{
	char** matches = NULL;
	matches = rl_completion_matches(text, cmd_generator);
	return matches;
}

static std::string readcli()
{
	static char* line = NULL;
	const char* prompt = g_prompt;

	if (1 == g_cli_state) // 多行输入
	{
		prompt = ">";
	}

	if (line) {
		free(line);
		line = NULL;
	}

	line = readline(prompt);

	std::string line_str = delete_extra_space(line);

	if (line_str.empty()) {
		return "";
	}

	if (line_str.back() == '\\') { // 多行输入
		line_str.pop_back();
		if (line_str.back() != ' ') {
			line_str.push_back(' ');
		}

		g_input_buffer.append(line_str);
		g_cli_state = 1;
		return "";
	}

	g_input_buffer.append(line_str);
	g_cli_state = 0;

	line_str = g_input_buffer;
	g_input_buffer.clear();

	if (!line_str.empty()) {
		add_history(line_str.c_str());
	}

	return line_str;
}

static void init_cli(const char* prompt = nullptr, std::map<std::string, std::vector<const char*>>* cmds = nullptr) {
	if (prompt && strlen(prompt) != 0) {
		g_prompt = prompt;
	}

	if (cmds) {
		g_cmd_map = *cmds;
	}

	rl_attempted_completion_function = cmd_completion;	
}

}
