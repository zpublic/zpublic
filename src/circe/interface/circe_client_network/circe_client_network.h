#ifdef CIRCE_CLIENT_NETWORK_EXPORTS
#define CIRCE_CLIENT_NETWORK_API __declspec(dllexport)
#else
#define CIRCE_CLIENT_NETWORK_API __declspec(dllimport)
#endif

typedef long (*pf_circe_client_network_version)(void);
typedef bool (*pf_circe_client_network_interface)(IID iid, PVOID *pv);

extern "C"
{
    CIRCE_CLIENT_NETWORK_API long circe_client_network_version(void);
    CIRCE_CLIENT_NETWORK_API bool circe_client_network_interface(IID iid, PVOID *pv);
}
