#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <optional>
#include <algorithm>
#include <sstream>

struct Node {
	std::string id;
	std::string type;
	std::string resource;
	int yield = 0;
	int gatherTimeTicks = 2;
};

struct Town {
	std::string name;

	int productionRateTicks = 10;
	std::unordered_map<std::string, int> productionResources;

	int enteloopRaeTicks = 10;
	int enteloopAmount = 0;

};

// Forward-declare Route (may be defined elsewhere)
struct Route
{
	std::string a, b;
	int weight = 0;
};

struct World {
	std::unordered_map<std::string, Node> nodes;
	std::unordered_map<std::string, Town> towns;
	// Function to get a node by its ID
	std::vector<Route> routes;
	// total available ticks in the world (used by Mayor)
	int totalTicks = 0;

	static const std::unordered_map<std::string, int>& sellPrices() {
		static const std::unordered_map<std::string, int> prices = {
			{"wheat", 2}, {"wood", 3}, {"stone", 3},
			{"clay", 4}, {"fish", 4}, {"sheep", 5}

		};
		return prices;
	}

	static const std::unordered_map<std::string, int>& buyPrices() {
		static const std::unordered_map<std::string, int> prices = {
			{"wheat", 4}, {"wood", 5}, {"stone", 5},
			{"clay", 6}, {"fish", 6}, {"sheep", 8}
		};
		return prices;
	}

	bool isTown(const std::string& vertex) const { return towns.count(vertex) > 0; }
	bool isNode(const std::string& vertex) const { return nodes.count(vertex) > 0; }


};

struct Action {
	std::string type;
	// optional fields: present depending on action.type
	std::string destination;
	std::string item;
	int quantity;
};

class Mayor {
public:
	Mayor(World& world, const std::string& startingTown, int startingEnteloot)
		: world_(world),
		currentLocation_(startingTown),
		tick_(0),
		enteloot_(startingEnteloot) {
		if (!world_.isTown(startingTown)) {
			throw std::invalid_argument("Invalid starting town");
		}
	}

	int currentTick() const { return tick_; }
	long long enteloot() const { return enteloot_; }
	const std::string& location() const { return currentLocation_; }
	const std::unordered_map<std::string, long long>& inventory() const { return inventory_; }
	const std::vector<std::string>& log() const { return log_; }

	void runActions(const std::vector<Action>& actions) {
		for (const auto& action : actions) {
			if (tick_ >= world_.totalTicks) {
				break;
			}
			dispatch(action);
		}
		if (tick_ < world_.totalTicks) {
			tick_ = world_.totalTicks;
		}
	}

	bool travel(const std::string& destination) {
		const Route* route = findRoute(currentLocation_, destination);
		if (!route) return false;
		if (!spendTicks(route->weight)) return false;

		currentLocation_ = destination;
		return true;
	}

	bool gather() {
		auto it = world_.nodes.find(currentLocation_);
		if (it == world_.nodes.end()) return false;
		const Node& node = it->second;

		if (!spendTicks(node.gatherTimeTicks)) return false;

		inventory_[node.resource] += node.yield;
		return true;
	}

	bool buy(const std::string& item, int quantity) {
		if (quantity <= 0) return false;
		auto townIt = world_.towns.find(currentLocation_);
		if (townIt == world_.towns.end()) return false;
		const Town& town = townIt->second;

		if (town.productionResources.find(item) == town.productionResources.end()) return false;

		const auto& prices = World::buyPrices();
		auto priceIt = prices.find(item);
		if (priceIt == prices.end()) return false;

		long long cost = static_cast<long long>(priceIt->second) * quantity;
		if (cost > enteloot_) return false;
		if (!spendTicks(1)) return false;

		enteloot_ -= cost;
		inventory_[item] += quantity;
		return true;
	}

	bool sell(const std::string& item, int quantity) {
		if (quantity <= 0) return false;
		auto invIt = inventory_.find(item);
		if (invIt == inventory_.end() || invIt->second < quantity) return false;
		const auto& sellPrices = World::sellPrices();
		auto priceIt = sellPrices.find(item);
		if (priceIt == sellPrices.end()) return false;

		long long revenue = static_cast<long long>(priceIt->second) * quantity;

		if (!spendTicks(1)) return false;
		invIt->second -= quantity;
		enteloot_ += revenue;
		return true;
	}

private:
	World& world_;
	std::string currentLocation_;
	int tick_;
	long long enteloot_;
	std::unordered_map<std::string, long long> inventory_;
	std::vector<std::string> log_;

	void dispatch(const Action& action) {
		bool ok = false;
		if (action.type == "travel" && !action.destination.empty()) {
			ok = travel(action.destination);
		}
		else if (action.type == "gather") {
			ok = gather();
		}
		else if (action.type == "buy" && !action.item.empty() && action.quantity > 0) {
			ok = buy(action.item, action.quantity);
		}
		else if (action.type == "sell" && !action.item.empty() && action.quantity > 0) {
			ok = sell(action.item, action.quantity);
		}
		else {
			ok = false;
		}

		if (!ok) {
			spendTicks(1);
			log_.push_back("INVALID: " + describe(action));
		}
		else {
			log_.push_back("SUCCESS: " + describe(action));
		}
	}

	bool spendTicks(int cost) {
		if (tick_ >= world_.totalTicks) return false;
		if (tick_ + cost > world_.totalTicks) return false;
		tick_ += cost;
		return true;
	}

	const Route* findRoute(const std::string& from, const std::string& to) const {
		for (const auto& r : world_.routes) {
			if ((r.a == from && r.b == to) || (r.a == to && r.b == from)) return &r;
		}

		return nullptr;
	}

	static std::string describe(const Action& a) {
		std::ostringstream oss;
		oss << a.type;
		if (!a.destination.empty()) oss << "->" << a.destination;
		if (!a.item.empty()) oss << " item=" << a.item;
		if (a.quantity > 0) oss << " qty=" << a.quantity;
		return oss.str();
	}
};

