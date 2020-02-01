namespace ptl
{
    template<typename T>
    class MemoryLinkedList
    {
    public:
        struct Node {
            T data{nullptr};
            Node* next{ nullptr };
        };
        Node* head;
    public:
            Node* nodes[5];
        size_t id = 0;
        MemoryLinkedList() = default;
        MemoryLinkedList(MemoryLinkedList &stackLinkedList) = delete;

        void push(Node * newNode, void* data);
        Node* pop();
    };

    template <class T>
    void MemoryLinkedList<T>::push(Node * newNode,void* data) {
        newNode->next = head;
        newNode->data = data;
        nodes[id] = newNode;
        head = newNode;
        ++id;
    }

    template <class T>
    typename MemoryLinkedList<T>::Node* MemoryLinkedList<T>::pop() {
        Node * top = head;
        head = head->next;
        return top;
    }
}