# Spécification CAN — eBus BMS

## Informations générales

| Paramètre | Valeur             |
| --------- | ------------------ |
| Débit     | 500 kbit/s         |
| Format ID | Standard (11 bits) |
| Boutisme  | Little Endian      |
| Version   | 2.0                |
| Date      | 27/02/2026         |

## Liste des messages

| ID (hex) | Nom                | Émetteur | Récepteur | DLC (bytes) | Périodicité  | Description                       |
| -------- | ------------------ | -------- | --------- | ----------- | ------------ | --------------------------------- |
| 0x100    | BATTERY_STATUS     | Batterie | BMS       | 8           | 100 ms       | Tension et courant de la batterie |
| 0x200    | TEMPERATURE_STATUS | Batterie | BMS       | 4           | 100 ms       | Température de la batterie        |
| 0x050    | ALERT_WARNING      | BMS      | Chargeur  | 1           | Événementiel | Alerte vigilance                  |
| 0x025    | ALERT_CRITICAL     | BMS      | Chargeur  | 1           | Événementiel | Alerte critique                   |

---

## Détail des messages

### 0x200 — TEMPERATURE_STATUS

**Émetteur :** Carte Batterie → **Récepteur :** Carte BMS
**DLC :** 4 octets | **Périodicité :** 100 ms

| Signal      | Octet départ | Longueur (bits) | Type           | Min | Max | Unité | Description |
| ----------- | ------------ | --------------- | -------------- | --- | --- | ----- | ----------- |
| Temperature | 0            | 32              | Float IEEE 754 | -40 | 85  | °C    | Température |

> **Exemple de décodage :**
> Octets reçus : `[0x00, 0x00, 0xC8, 0x41]`
>
> - Temperature = octets [0:3] = 0x41C80000 (little-endian) → **25.0 °C**

### 0x100 — BATTERY_STATUS

**Émetteur :** Carte Batterie → **Récepteur :** Carte BMS
**DLC :** 8 octets | **Périodicité :** 100 ms

| Signal    | Octet départ | Longueur (bits) | Type           | Min   | Max  | Unité | Description |
| --------- | ------------ | --------------- | -------------- | ----- | ---- | ----- | ----------- |
| Intensity | 0            | 32              | Float IEEE 754 | -1000 | 1000 | A     | Courant     |
| Voltage   | 4            | 32              | Float IEEE 754 | 0     | 1000 | V     | Tension     |

> **Exemple de décodage :**
> Octets reçus : `[0x00, 0x00, 0x20, 0x41, 0x00, 0x00, 0x40, 0x42]`
>
> - Intensity = octets [0:3] = 0x41200000 (little-endian) → **10.0 A**
> - Voltage = octets [4:7] = 0x42400000 (little-endian) → **48.0 V**


### 0x050 — ALERT_WARNING

**Émetteur :** Carte BMS → **Récepteur :** Carte Chargeur
**DLC :** 1 octets | **Périodicité :** Événementiel

| Signal     | Bit départ | Longueur (bits) | Type     | Facteur | Offset | Min | Max | Unité | Description                           |
| ---------- | ---------- | --------------- | -------- | ------- | ------ | --- | --- | ----- | ------------------------------------- |
| Alert_Code | 0          | 8               | Unsigned | 1       | 0      | 0   | 255 | -     | Code d'erreur (voir table ci-dessous) |

**Table des codes d'alerte :**

| Code | Description   |
| ---- | ------------- |
| 0x01 | Surtension    |
| 0x02 | Sous-tension  |
| 0x03 | Surintensité  |
| 0x04 | Sousintensité |
| 0x05 | Surchauffe    |

### 0x025 — ALERT_CRITICAL

**Émetteur :** Carte BMS → **Récepteur :** Carte Chargeur
**DLC :** 1 octets | **Périodicité :** Événementiel

| Signal     | Bit départ | Longueur (bits) | Type     | Facteur | Offset | Min | Max | Unité | Description                           |
| ---------- | ---------- | --------------- | -------- | ------- | ------ | --- | --- | ----- | ------------------------------------- |
| Alert_Code | 0          | 8               | Unsigned | 1       | 0      | 0   | 255 | -     | Code d'erreur (voir table ci-dessous) |

**Table des codes d'alerte (évolutif):**

| Code | Description   |
| ---- | ------------- |
| 0x01 | Surtension    |
| 0x02 | Sous-tension  |
| 0x03 | Surintensité  |
| 0x04 | Sousintensité |
| 0x05 | Surchauffe    |

## Notes

- Les valeurs physiques sont encodées en Float IEEE 754 simple précision (32 bits), little-endian
- Convention de signe courant : positif = charge, négatif = décharge
