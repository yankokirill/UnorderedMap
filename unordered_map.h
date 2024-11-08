#include <iostream>
#include <memory>
#include <iterator>

template <typename Key, typename Value, typename Hash, typename KeyEqual, typename Allocator>
class ForwardList {
protected:
    using value_type = std::pair<const Key, Value>;
    struct BaseNode {
        BaseNode* next = nullptr;
    };

    struct Node : BaseNode {
        std::pair<Key, Value> data;
        size_t hash;
    };

    size_t get_hash(BaseNode* it) {
        return static_cast<Node*>(it)->hash;
    }

    Key& get_key(BaseNode* it) {
        return static_cast<Node*>(it)->data.first;
    }

    value_type& get_data(BaseNode* it) {
        return reinterpret_cast<value_type&>(static_cast<Node*>(it)->data);
    }

    using Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<value_type>;
    using NodeAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

    [[no_unique_address]] Hash hash_func;
    [[ no_unique_address ]] KeyEqual cmp_equal;

    [[ no_unique_address ]] Alloc alloc;
    [[ no_unique_address ]] NodeAlloc node_alloc;

    BaseNode fake_node;
    size_t sz = 0;

    void delete_node(BaseNode* ptr) {
        Node* it = static_cast<Node*>(ptr);
        std::allocator_traits<Alloc>::destroy(alloc, &get_data(it));
        std::allocator_traits<NodeAlloc>::deallocate(node_alloc, it, 1);
    }

private:
    template <typename... Args>
    Node* place_construct(Args&&... args) {
        Node* ptr = std::allocator_traits<NodeAlloc>::allocate(node_alloc, 1);
        try {
            std::allocator_traits<Allocator>::construct(alloc, &get_data(ptr), std::forward<Args>(args)...);
        } catch (...) {
            std::allocator_traits<NodeAlloc>::deallocate(node_alloc, ptr, 1);
            throw;
        }
        return ptr;
    }

protected:
    Node* copy_node(BaseNode* copy, BaseNode* next = nullptr) {
        Node* ptr = place_construct(get_data(copy));
        ptr->next = next;
        ptr->hash = get_hash(copy);
        return ptr;
    }

    template <typename... Args>
    Node* emplace_new_node(Args&&... args) {
        Node* ptr = place_construct(std::forward<Args>(args)...);
        try {
            ptr->hash = hash_func(ptr->data.first);
        } catch (...) {
            delete_node(ptr);
            throw;
        }
        return ptr;
    }

    BaseNode* insert_next(BaseNode* it, BaseNode* elem) {
        elem->next = it->next;
        it->next = elem;
        return elem;
    }

    void destroy() {
        BaseNode* it = fake_node.next;
        while (it) {
            BaseNode* tmp = it;
            it = it->next;
            delete_node(tmp);
        }
    }

    ForwardList() {}

    explicit ForwardList(const Allocator& alloc) :  alloc(alloc),
                                                    node_alloc(alloc) {
    }


    ForwardList(const ForwardList& copy, const Allocator& alloc) : hash_func(copy.hash_func),
                                                                   cmp_equal(copy.cmp_equal),
                                                                   alloc(alloc),
                                                                   node_alloc(alloc),
                                                                   sz(copy.sz) {
        if (sz == 0) {
            return;
        }
        try {
            BaseNode *last = &fake_node;
            BaseNode *it = copy.fake_node.next;
            last->next = copy_node(it);
            last = last->next;

            for (it = it->next; it; it = it->next) {
                last->next = copy_node(it);
                last = last->next;
            }
        } catch (...) {
            destroy();
            throw;
        }
    }

    ForwardList(const ForwardList& copy) : ForwardList(copy,
        std::allocator_traits<NodeAlloc>::select_on_container_copy_construction(copy.alloc)) {}

private:
    void moveList(ForwardList&& copy) {
        destroy();

        alloc = std::move(copy.alloc);
        node_alloc = std::move(copy.node_alloc);

        cmp_equal = std::move(copy.cmp_equal);
        hash_func = std::move(copy.hash_func);
        fake_node.next = copy.fake_node.next;
        sz = copy.sz;

        copy.fake_node.next = nullptr;
        copy.sz = 0;
    }

protected:
    ForwardList(ForwardList&& copy) {
        moveList(std::move(copy));
    }

    ForwardList& operator=(const ForwardList& copy) {
        ForwardList res(copy, std::allocator_traits<Allocator>::propagate_on_container_copy_assignment::value ?
                              copy.alloc : alloc);
        fake_node.next = res.fake_node.next;
        res.fake_node.next = nullptr;
        return *this;
    }

    ForwardList& operator=(ForwardList&& copy) noexcept {
        if (this == &copy) {
            return *this;
        }
        moveList(std::move(copy));
        return *this;
    }

    template <typename U>
    class BaseIterator {
    public:
        using value_type = U;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = ptrdiff_t;
        using const_reference = const value_type &;
        using iterator_category = std::forward_iterator_tag;

    public:
        BaseNode* item;

        BaseIterator(BaseNode* item) : item(item) {}

        BaseIterator() = default;

        operator BaseIterator<const U>() const {
            return BaseIterator<const U>(item);
        }

        value_type& operator*() const {
            return reinterpret_cast<value_type&>(static_cast<Node*>(item)->data);
        }

        value_type* operator->() const {
            return &operator*();
        }

        BaseIterator& operator++() {
            item = item->next;
            return *this;
        }

        BaseIterator operator++(int) {
            auto copy = *this;
            ++*this;
            return copy;
        }

        template <typename P>
        bool operator==(const BaseIterator<P>& other) const {
            return item == other.item;
        }

    };
    using iterator = BaseIterator<value_type>;
    using const_iterator = BaseIterator<const value_type>;

    ~ForwardList() {
        destroy();
    }
};

template <typename Key,
        typename Value,
        typename Hash = std::hash<Key>,
        typename KeyEqual = std::equal_to<Key>,
        typename Allocator = std::allocator<std::pair<const Key, Value>>>
class UnorderedMap : protected ForwardList<Key, Value, Hash, KeyEqual, Allocator> {
private:
    using List = ForwardList<Key, Value, Hash, KeyEqual, Allocator>;
public:
    using value_type = std::pair<const Key, Value>;
    using key_type = Key;
    using mapped_type = Value;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using hasher = Hash;
    using key_equal = KeyEqual;
    using allocator_type = Allocator;
    using reference = value_type &;
    using const_reference = const value_type&;

    using iterator = typename List::iterator;
    using const_iterator =  typename List::const_iterator;

private:
    using List::hash_func;
    using List::cmp_equal;
    using List::fake_node;
    using List::sz;

    using BaseNode = typename List::BaseNode;
    using Node = typename List::Node;

    using List::get_data;
    using List::get_key;
    using List::insert_next;
    using List::delete_node;
    using List::emplace_new_node;

    using NodePtrAlloc = typename std::allocator_traits<Allocator>::template rebind_alloc<BaseNode*>;
    [[ no_unique_address ]] NodePtrAlloc node_ptr_alloc;

    size_t bucket_count = 1;
    BaseNode** arr = std::allocator_traits<NodePtrAlloc>::allocate(node_ptr_alloc, bucket_count);

    float max_load = 1.0;

    size_t get_hash(BaseNode* it) {
        return List::get_hash(it) % bucket_count;
    }

    void fixed_rehash(size_t count) {
        auto new_arr = std::allocator_traits<NodePtrAlloc>::allocate(node_ptr_alloc, count);
        std::allocator_traits<NodePtrAlloc>::deallocate(node_ptr_alloc, arr, bucket_count);
        arr = new_arr;
        bucket_count = count;
        std::fill(arr, arr + bucket_count, nullptr);
        BaseNode* last = &fake_node;
        for (BaseNode* it = fake_node.next; it;) {
            BaseNode* cur = it;
            it = it->next;
            size_t ind  = get_hash(cur);
            if (arr[ind]) {
                insert_next(arr[ind], cur);
            } else {
                arr[ind] = last;
                last->next = cur;
                last = cur;
            }
        }
        last->next = nullptr;
    }

    void reallocate() {
        if (load_factor() > max_load) {
            fixed_rehash(2 * bucket_count);
        }
    }

public:
    UnorderedMap() {
        arr[0] = nullptr;
    }

    UnorderedMap(const Allocator &alloc) : List(alloc) {
        std::fill(arr, arr + bucket_count, nullptr);
    }

    UnorderedMap(const UnorderedMap& copy, const Allocator& alloc) :
            List(copy, alloc),
            bucket_count(copy.bucket_count),
            max_load(copy.max_load) {
        std::fill(arr, arr + bucket_count, nullptr);
    }

    UnorderedMap(const UnorderedMap& copy) : UnorderedMap(copy,
        std::allocator_traits<Allocator>::select_on_container_copy_construction(copy.node_alloc)) {}

    UnorderedMap(UnorderedMap&& copy) : List(std::move(copy)),
                                        node_ptr_alloc(std::move(copy.node_ptr_alloc)),
                                        bucket_count(copy.bucket_count),
                                        arr(copy.arr),
                                        max_load(copy.max_load) {
        copy.arr = nullptr;
        copy.bucket_count = 0;
    }

    UnorderedMap& operator=(const UnorderedMap& copy) {
        if (&copy == this) {
            return *this;
        }
        auto new_arr = std::allocator_traits<NodePtrAlloc>::allocate(node_ptr_alloc, copy.bucket_count);
        try {
            List::operator=(copy);
        } catch (...) {
            std::allocator_traits<NodePtrAlloc>::deallocate(node_ptr_alloc, new_arr, copy.bucket_count);
            throw;
        }
        std::allocator_traits<NodePtrAlloc>::deallocate(node_ptr_alloc, arr, bucket_count);
        bucket_count = copy.bucket_count;
        arr = new_arr;
        std::copy(copy.arr, copy.arr + bucket_count, arr);
        max_load = copy.max_load;
        return *this;
    }

    UnorderedMap& operator=(UnorderedMap&& copy) {
        if (&copy == this) {
            return *this;
        }
        List::operator=(std::move(copy));
        std::allocator_traits<NodePtrAlloc>::deallocate(node_ptr_alloc, arr, bucket_count);
        bucket_count = copy.bucket_count;
        arr = copy.arr;
        max_load = copy.max_load;
        copy.arr = nullptr;
        copy.bucket_count = 0;
        return *this;
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    float load_factor() const {
        return static_cast<float>(sz) / static_cast<float>(bucket_count);
    }

    float max_load_factor() const {
        return max_load;
    }

    void max_load_factor(float ml) {
        max_load = ml;
        if (load_factor() > max_load) {
            size_t count = static_cast<size_t>(static_cast<float>(sz) / max_load) + 1;
            fixed_rehash(count);
        }
    }

    void rehash(size_t count) {
        count = std::max(count, static_cast<size_t>(static_cast<float>(sz) / max_load) + 1);
        fixed_rehash(count);
    }

    void reserve(size_t count) {
        count = static_cast<size_t>(static_cast<float>(count) / max_load) + 1;
        if (count > bucket_count) {
            fixed_rehash(count);
        }
    }

    void swap(UnorderedMap& other) {
        if constexpr (std::allocator_traits<Allocator>::propagate_on_container_swap::value) {
            std::swap(List::alloc, List::other.alloc);
            std::swap(List::node_alloc, List::other.node_alloc);
            std::swap(node_ptr_alloc, other.node_ptr_alloc);
        }
        std::swap(cmp_equal, other.cmp_equal);
        std::swap(fake_node, other.fake_node);
        std::swap(sz, other.sz);

        std::swap(bucket_count, other.bucket_count);
        std::swap(arr, other.arr);

        std::swap(hash_func, other.hash_func);
        std::swap(max_load, other.max_load);
    }

    iterator begin() {
        return iterator(fake_node.next);
    }
    iterator end() {
        return iterator(nullptr);
    }

    const_iterator begin() const {
        return const_iterator(fake_node.next);
    }
    const_iterator end() const {
        return const_iterator(nullptr);
    }

    const_iterator cbegin() const {
        return const_iterator(fake_node.next);
    }
    const_iterator cend() const {
        return const_iterator(nullptr);
    }

private:
    BaseNode* find_node(const Key& key, size_t hash) {
        if (!arr[hash]) {
            return nullptr;
        }
        BaseNode* it = arr[hash]->next;
        while (it && get_hash(it) == hash && !cmp_equal(get_data(it).first, key)) {
            it = it->next;
        }
        if (it && get_hash(it) == hash) {
            return it;
        } else {
            return nullptr;
        }
    }
    BaseNode* find_node(const Key& key) {
        return find_node(key, hash_func(key) % bucket_count);
    }

public:
    iterator find(const Key& key) {
        return iterator(find_node(key));
    }

    const_iterator find(const Key& key) const {
        return const_iterator(find(key));
    }

    template <typename Pair>
    std::pair<iterator, bool> insert(Pair&& value) {
        return emplace(std::forward<Pair>(value));
    }

    std::pair<iterator, bool> insert(const value_type& value) {
        return insert<const_reference>(value);
    }

    template <typename InputIt>
    void insert(InputIt first, InputIt last) {
        auto it = first;
        try {
            for (; it != last; ++it) {
                insert(*it);
            }
        } catch (...) {
            for (; first != it; ++first) {
                auto iter = find(first->first);
                if (iter.item) {
                    erase(iter);
                }
            }
            throw;
        }
    }

public:
    template<class... Args>
    std::pair<iterator, bool> emplace(Args &&... args) {
        BaseNode* elem = emplace_new_node(std::forward<Args>(args)...);
        size_t hash = get_hash(elem);
        BaseNode* it = find_node(get_key(elem), hash);
        if (it) {
            delete_node(elem);
            return {iterator(it), false};
        }
        if (!arr[hash]) {
            if (fake_node.next) {
                arr[get_hash(fake_node.next)] = elem;
            }
            arr[hash] = &fake_node;
        }
        insert_next(arr[hash], elem);
        ++sz;
        reallocate();
        return {iterator(elem), true};
    }

    iterator erase(const_iterator pos) {
        size_t hash = get_hash(pos.item);
        BaseNode* it = arr[hash];
        while (it->next != pos.item) {
            it = it->next;
        }
        BaseNode* next_elem = pos.item->next;
        it->next = next_elem;
        if (next_elem && get_hash(next_elem) != hash) {
            arr[get_hash(next_elem)] = it;
        }
        it = arr[hash]->next;
        if (!it || get_hash(it) != hash) {
            arr[hash] = nullptr;
        }
        delete_node(pos.item);
        --sz;
        return iterator(next_elem);
    }

    void erase(const_iterator first, const_iterator last) {
        for (iterator it(first.item); it != last; it = erase(it)) {}
    }

    Value& operator[](const Key& key) {
        iterator it = emplace(key, Value()).first;
        return it->second;
    }

    Value& operator[](Key&& key) {
        iterator it = emplace(std::forward<Key>(key), Value()).first;
        return it->second;
    }


    Value& at(const Key& key) {
        iterator it = find(key);
        if (it.item) {
            return it->second;
        } else {
            throw std::out_of_range("Key doesn't exist");
        }
    }

    Value& at(const Key& key) const {
        const_iterator it = find(key);
        if (it.item) {
            return it->second;
        } else {
            throw std::out_of_range("Key doesn't exist");
        }
    }

    bool operator==(const UnorderedMap& other) {
        if (sz != other.sz) {
            return false;
        }
        for (auto it = begin(); it != end(); ++it) {
            if (other.find(*it) == other.end()) {
                return false;
            }
        }
        return true;
    }

    ~UnorderedMap() {
        std::allocator_traits<NodePtrAlloc>::deallocate(node_ptr_alloc, arr, bucket_count);
    }
};
