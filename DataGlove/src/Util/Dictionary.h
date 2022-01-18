#ifndef DICTIONARY
#define DICTIONARY
#include <unordered_map>

template <class K, class V>
	class Dictionary : public std::unordered_map<K, V> {
	public:
		Dictionary() = default;
		~Dictionary() = default;

		/// <summary>
		/// Tries to retrieve the value at the specified key
		/// </summary>
		/// <param name="key">Key where a value must be found for</param>
		/// <param name="output">The object reference to copy found value into</param>
		/// <returns><c>true</c> if value was found for key, <c>false</c> if key was not found for key</returns>
		inline bool try_get_value(K key, V &output) {
			bool result;
			try {
				output = this->at(key);
				result = true;
			}
			catch (std::exception) {
				result = false;
			}
			return result;
		}

		/// <summary>
		/// Tries to erase the key and value at the specified key
		/// </summary>
		/// <param name="key">The key to erase</param>
		/// <returns><c>true</c> if value was erased, <c>false</c> if Dictionary did not contain the key</returns>
		inline bool try_erase(K key) {
			try {
				this->erase(key);
				return true;
			}
			catch (std::exception eor) {
				return false;
			}
		}
	};
#endif